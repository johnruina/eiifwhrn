#ifndef BOX_CLASS
#define BOX_CLASS

#include <iostream>

#include "common.h"
//#include "Engine.h"
#include "VAO.h"
#include "VBO.h"
#include "QuadVertices.h"
#include "shaderClass.h"
#include "texture.h"
#include "t2d.h"
#include "font.h"
#include "RenderSystem.h"
#include "Object.h"

class Box : public Renderable, public Object {
public:

    VAO boxVAO;
    VBO boxVBO;

    glm::vec3 Color = {1.0f,1.0f,1.0f};
    float Opacity = 1.0f;

    t2d_package t2d;

    float z = 0.0f;

    float rotation = 0.0f;  

    float rounding = 0.0f;

    Box() {
        shadertype = ShaderType::BoxShader;
        boxVBO.GenerateID();
        boxVAO.GenerateID();
        boxVAO.Bind();
        boxVBO.BufferData(&quadVertices, sizeof(quadVertices));

        boxVAO.LinkVBO(boxVBO, 0, 2, GL_FLOAT, 4 * sizeof(float), (void*)0);
        boxVAO.LinkVBO(boxVBO, 1, 2, GL_FLOAT, 4 * sizeof(float), (void*)(2 * sizeof(float)));
    }

    virtual void Render(Shader& ShaderProgram) override {
        if (Opacity <= 0.0f) return;
        ShaderProgram.Activate();
        //UNIFORMS
        t2d.Recalculate();
        ShaderProgram.Set4F("Color", { Color,Opacity });
        ShaderProgram.Set2F("normalizedCenterScale", t2d.NormalizedCenterScale);
        ShaderProgram.Set2F("normalizedCenterPos", t2d.NormalizedCenterPos);
        ShaderProgram.Set1F("z", z);
        ShaderProgram.Set1F("rotation", rotation);
        ShaderProgram.Set2F("pixelScale", t2d.pixelsize);
        ShaderProgram.Set2F("pixelPos", t2d.pixelposition);
        ShaderProgram.Set1F("rounding", rounding);

        //render
        boxVAO.Bind();
        glDrawArrays(GL_TRIANGLES, 0, 6);
        boxVAO.Unbind();
    }
};

class BoxButton : public Box{
public:

    bool clicked = false;

    void UpdateClicked(float mousex, float mousey) {
        //IMPLEMENT ROUNDING SOMETIME
        if (mousex > t2d.pixelposition.x and mousey > t2d.pixelposition.y and mousex < t2d.pixelposition.x + t2d.pixelsize.x and mousey < t2d.pixelposition.y + t2d.pixelsize.y) {
            clicked = true;
            std::cout << "CLICKED\n";
        }
        else {
            clicked = false;
        }
    }

    BoxButton() {
    }

};

class TextBox : public Box {
public:
    Font* font = nullptr;
    std::string text;
    float fontsize = 1.0f;
    glm::vec2 textCenter = { 0.0f, 0.0f }; // 0,0 = top-left, 0.5,0.5 = center, 1,1 = bottom-right

    TextBox() {
        shadertype = ShaderType::TextShader;
    }

    virtual void Render(Shader& ShaderProgram) override {
        if (!font or Opacity <= 0.0f) return;
        t2d.Recalculate();
        // Calculate total text width and height for centering
        float totalWidth = 0.0f;
        float maxHeight = 0.0f;
        for (const char& c : text) {
            TextCharacter ch = font->Characters[(unsigned char)c];
            totalWidth += (ch.Advance >> 6) * fontsize;
            maxHeight = glm::max(maxHeight, (float)ch.Size.y * fontsize);
        }

        float x = t2d.pixelposition.x + (t2d.pixelsize.x - totalWidth) * textCenter.x;
        float y = t2d.pixelposition.y + (t2d.pixelsize.y - maxHeight) * textCenter.y;

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        ShaderProgram.Activate();
        ShaderProgram.Set4F("textColor", { Color, Opacity });
        ShaderProgram.Set2F("screenSize", glm::vec2(window->width, window->height));
        ShaderProgram.SetInt("text", 0);
        glActiveTexture(GL_TEXTURE0);
        font->vao.Bind();

        for (const char& c : text) {
            TextCharacter ch = font->Characters[(unsigned char)c];
            if (ch.Size.x == 0 || ch.Size.y == 0) {
                x += (ch.Advance >> 6) * fontsize;
                continue;
            }

            float x0 = x + ch.Bearing.x * fontsize;
            float y0 = y + (ch.Size.y - ch.Bearing.y) * fontsize;
            float x1 = x0 + ch.Size.x * fontsize;
            float y1 = y0 - ch.Size.y * fontsize;

            float vertices[6][4] = {
                { x0, y1, 0.0f, 0.0f },
                { x0, y0, 0.0f, 1.0f },
                { x1, y0, 1.0f, 1.0f },
                { x0, y1, 0.0f, 0.0f },
                { x1, y0, 1.0f, 1.0f },
                { x1, y1, 1.0f, 0.0f }
            };

            glBindTexture(GL_TEXTURE_2D, ch.TextureID);
            font->vbo.Bind();
            glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
            font->vbo.Unbind();

            glDrawArrays(GL_TRIANGLES, 0, 6);

            x += (ch.Advance >> 6) * fontsize;
        }

        font->vao.Unbind();
        glBindTexture(GL_TEXTURE_2D, 0);
    }
};
class ImageBox : public Box {
public:
    Texture* tex;
    ImageBox() {
        shadertype = ShaderType::ImageBoxShader;
    }

    virtual void Render(Shader& ShaderProgram) override {
        if (Opacity <= 0.0f) return;

        ShaderProgram.Activate();
        //UNIFORMS

        t2d.Recalculate();
        ShaderProgram.Set4F("Color", { Color,Opacity });
        ShaderProgram.Set2F("normalizedCenterScale", t2d.NormalizedCenterScale);
        ShaderProgram.Set2F("normalizedCenterPos", t2d.NormalizedCenterPos);
        ShaderProgram.Set1F("z", z);
        ShaderProgram.Set1F("rotation", rotation);
        ShaderProgram.Set2F("pixelScale", t2d.pixelsize);
        ShaderProgram.Set2F("pixelPos", t2d.pixelposition);
        ShaderProgram.Set1F("rounding", rounding);

        glActiveTexture(0);

        if (tex != nullptr) {
            glBindTexture(GL_TEXTURE_2D, tex->ID);
            ShaderProgram.SetInt("tex", 0);
        }
        else glBindTexture(GL_TEXTURE_2D, 0);

        //render
        boxVAO.Bind();
        glDrawArrays(GL_TRIANGLES, 0, 6);
        boxVAO.Unbind();
    }

};

#endif>