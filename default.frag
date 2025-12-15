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

void main()
{
    vec4 lightless = (texture(tex0,texCoord)) * vec4(color,1.0f);

    //vec4 ambient = lightless * vec4(material.ambient,1.0f);

    if (lightless.a < 0.1) {
        discard;
    }

    FragColor = lightless * vec4(vec3(1.0f-1.0f/gl_FragCoord[3]/ 200.0f), 1.0f);
}