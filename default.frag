#version 330 core
out vec4 FragColor;

in vec3 normal;
in vec2 texCoord;
in vec3 FragPos;

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;    
    float shininess;
};

struct PointLight {    
    vec3 position;
    
    float constant;
    float linear;
    float quadratic;  

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};  

struct DirLight {
    vec3 direction;
  
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};  

uniform DirLight dirLight;

uniform Material material;
uniform sampler2D tex0;
uniform vec3 viewPos;

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir)
{
    vec3 lightDir = normalize(-light.direction);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    // combine results
    vec3 ambient  = light.ambient;
    vec3 diffuse  = light.diffuse  * diff;
    vec3 specular = light.specular * spec;
    return (ambient + diffuse + specular);
}  

void main()
{
    vec4 lightless = texture(tex0,texCoord);

    if (lightless.a < 0.1) {
        discard;
    }

    vec3 norm = normalize(normal);
    vec3 viewDir = normalize(viewPos - FragPos);

    FragColor = lightless * vec4(CalcDirLight(dirLight,norm,viewDir),1.0f) * vec4(vec3(1.0f-1.0f/gl_FragCoord[3]/ 200.0f), 1.0f);
}