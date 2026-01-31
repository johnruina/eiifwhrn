#version 330 core
layout (location = 0) in vec3 aPos;

out vec3 FragPos;

uniform mat4 modl;	
uniform mat4 view;
uniform mat4 proj;

void main()
{
    FragPos = vec3(modl * vec4(aPos, 1.0));
    gl_Position = proj* view * modl * vec4(aPos,1.0f);
}