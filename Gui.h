#ifndef BOX_CLASS
#define BOX_CLASS

#include <iostream>

#include "VAO.h"
#include "VBO.h"
#include "QuadVertices.h"
#include "shaderClass.h"
#include "texture.h"
#include "t2d.h"
#include "font.h"

class Box {
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
        boxVBO.BufferData(&quadVertices, sizeof(quadVertices));

        boxVAO.LinkVBO(boxVBO, 0, 2, GL_FLOAT, 4 * sizeof(float), (void*)0);
        boxVAO.LinkVBO(boxVBO, 1, 2, GL_FLOAT, 4 * sizeof(float), (void*)(2 * sizeof(float)));
    }

    virtual void Render(Shader& ShaderProgram, int& screenWidth, int& screenHeight) {
        ShaderProgram.Activate();
        //UNIFORMS
        t2d.Recalculate(screenWidth,screenHeight);
        ShaderProgram.Set4F("Color", { Color,Opacity });
        ShaderProgram.Set2F("normalizedCenterScale", t2d.NormalizedCenterScale);
        ShaderProgram.Set2F("normalizedCenterPos", t2d.NormalizedCenterPos);
        ShaderProgram.Set1F("z", z);
        ShaderProgram.Set1F("rotation", rotation);
        ShaderProgram.Set2F("screenSize", { screenWidth ,screenHeight });
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
//FAIRLY WIP, UNUSABLE AS OF CURRENT STATE
public:

    Font* font;
    std::string text;

    float fontsize = 32.0f;

    void RenderText(Shader& ShaderProgram, int& screenWidth, int& screenHeight) {

        ShaderProgram.Activate();
        glActiveTexture(GL_TEXTURE0);
        font->vao.Bind();

        // iterate through all characters

        float x = t2d.pixelposition.x;
        float y = t2d.pixelposition.y;

        std::string::const_iterator c;
        for (c = text.begin(); c != text.end(); c++)
        {
            Character ch = font->Characters[*c];

            float xpos = x + ch.Bearing.x * fontsize;
            float ypos = y - (ch.Size.y - ch.Bearing.y) * fontsize;

            float w = ch.Size.x * fontsize;
            float h = ch.Size.y * fontsize;
            // update VBO for each character
            float vertices[6][4] = {
                { xpos,     ypos + h,   0.0f, 0.0f },
                { xpos,     ypos,       0.0f, 1.0f },
                { xpos + w, ypos,       1.0f, 1.0f },

                { xpos,     ypos + h,   0.0f, 0.0f },
                { xpos + w, ypos,       1.0f, 1.0f },
                { xpos + w, ypos + h,   1.0f, 0.0f }
            };
            // render glyph texture over quad
            glBindTexture(GL_TEXTURE_2D, ch.TextureID);
            // update content of VBO memory
            font->vbo.Bind();
            glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
            glBindBuffer(GL_ARRAY_BUFFER, 0);
            // render quad
            glDrawArrays(GL_TRIANGLES, 0, 6);
            // now advance cursors for next glyph (note that advance is number of 1/64 pixels)
            x += (ch.Advance >> 6) * fontsize; // bitshift by 6 to get value in pixels (2^6 = 64)
        }
        glBindVertexArray(0);
        glBindTexture(GL_TEXTURE_2D, 0);
    }

    virtual void Render(Shader& ShaderProgram, int& screenWidth, int& screenHeight) {
        ShaderProgram.Activate();
        //UNIFORMS
        t2d.Recalculate(screenWidth, screenHeight);
        ShaderProgram.Set4F("Color", { Color,Opacity });
        ShaderProgram.Set2F("normalizedCenterScale", t2d.NormalizedCenterScale);
        ShaderProgram.Set2F("normalizedCenterPos", t2d.NormalizedCenterPos);
        ShaderProgram.Set1F("z", z);
        ShaderProgram.Set1F("rotation", rotation);
        ShaderProgram.Set2F("screenSize", { screenWidth ,screenHeight });
        ShaderProgram.Set2F("pixelScale", t2d.pixelsize);
        ShaderProgram.Set2F("pixelPos", t2d.pixelposition);
        ShaderProgram.Set1F("rounding", rounding);

        //render
        boxVAO.Bind();
        glDrawArrays(GL_TRIANGLES, 0, 6);
        boxVAO.Unbind();


    }

    TextBox() {
    }

};

class ImageBox : public Box {
public:
    Texture* tex;
    ImageBox() {

    }

    void Render(Shader& ShaderProgram, int& screenWidth, int& screenHeight) override {
        ShaderProgram.Activate();
        //UNIFORMS

        t2d.Recalculate(screenWidth,screenHeight);
        ShaderProgram.Set4F("Color", { Color,Opacity });
        ShaderProgram.Set2F("normalizedCenterScale", t2d.NormalizedCenterScale);
        ShaderProgram.Set2F("normalizedCenterPos", t2d.NormalizedCenterPos);
        ShaderProgram.Set1F("z", z);
        ShaderProgram.Set1F("rotation", rotation);
        ShaderProgram.Set2F("screenSize", { screenWidth ,screenHeight });
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