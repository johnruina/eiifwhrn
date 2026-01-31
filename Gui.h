#ifndef BOX_CLASS
#define BOX_CLASS

#include <iostream>

#include "VAO.h"
#include "VBO.h"
#include "QuadVertices.h"
#include "shaderClass.h"
#include "texture.h"

class Box {
public:
    VAO boxVAO;
    VBO boxVBO;

    glm::vec3 Color;
    float Opacity = 1.0f;
    glm::vec4 position;

    glm::vec4 scale;
    glm::vec2 center = { 0.0f,0.0f };

    Box() {
        boxVBO.BufferData(&quadVertices, sizeof(quadVertices));

        boxVAO.LinkVBO(boxVBO, 0, 2, GL_FLOAT, 4 * sizeof(float), (void*)0);
        boxVAO.LinkVBO(boxVBO, 1, 2, GL_FLOAT, 4 * sizeof(float), (void*)(2 * sizeof(float)));
    }

    virtual void Render(Shader& ShaderProgram) {
        ShaderProgram.Activate();
        //UNIFORMS
        ShaderProgram.Set4F("Color", { Color,Opacity });
        ShaderProgram.Set4F("scale", scale);
        ShaderProgram.Set4F("position", position);
        ShaderProgram.Set2F("center", center);
        glActiveTexture(0);

        //render
        boxVAO.Bind();
        glDrawArrays(GL_TRIANGLES, 0, 6);
        boxVAO.Unbind();
    }
};

class ImageBox : public Box {
public:
    Texture* tex;
    ImageBox() {

    }

    void Render(Shader& ShaderProgram) override {
		ShaderProgram.Activate();
        //UNIFORMS
        ShaderProgram.Set4F("Color", {Color,Opacity});
        ShaderProgram.Set4F("scale", scale);
        ShaderProgram.Set4F("position", position);
        ShaderProgram.Set2F("center", center);
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