#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTex;	

out vec3 normal;
out vec2 texCoord;

//out vec3 FragPos;

uniform mat4 modl;	
uniform mat4 view;
uniform mat4 proj;

void main()
{
	normal = aNormal;
    texCoord = aTex;
    gl_Position = proj* view * modl * vec4(aPos,1.0f);
}