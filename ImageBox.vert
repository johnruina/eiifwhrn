#version 330 core
layout (location = 0) in vec2 aPos;
layout (location = 1) in vec2 aTexCoords;

out vec2 TexCoords;

uniform float screenWidth = 1280.0f;
uniform float screenHeight = 720.0f;

uniform vec4 position;

uniform vec4 scale;

uniform vec2 center = {0.5f,0.5f};

void main()
{
    vec2 guiscale = scale.xy + scale.zw * 2/vec2(screenWidth,screenHeight);
    gl_Position = vec4(2.0f*position.xy + guiscale * (1 + aPos.xy -center * 2) - 1,  0.0, 1.0); 
    gl_Position.xy += position.zw * 2/vec2(screenWidth,screenHeight);
    TexCoords = aTexCoords;
}  