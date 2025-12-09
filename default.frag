#version 330 core
out vec4 FragColor;

in vec3 color;

in vec2 texCoord;

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;    
    float shininess;
};

uniform Material material;
uniform sampler2D tex0;
uniform mat4 view;

void main()
{
    vec4 lightless = (texture(tex0,texCoord)) * vec4(color,1.0f);

    vec4 ambient = lightless * vec4(material.ambient,0.0f);
    
    //float blackwhite = (lightless[0] + lightless[1] + lightless[2])/3;
    //FragColor = vec4(blackwhite,blackwhite,blackwhite,1.0f)* vec4(vec3(1.0f-1.0f/gl_FragCoord[3]/ 20.0f), 1.0f);
    
    FragColor = lightless;//* vec4(vec3(1.0f-1.0f/gl_FragCoord[3]/ 20.0f), 1.0f);
}