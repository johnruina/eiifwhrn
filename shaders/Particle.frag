#version 330 core
out vec4 FragColor;

in vec3 normal;
in vec2 texCoord;
in vec3 FragPos;

uniform sampler2D tex;
uniform vec4 color;

void main()
{
    FragColor =texture(tex,texCoord) * color;
}