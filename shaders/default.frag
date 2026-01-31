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

uniform vec3 viewPos;

uniform sampler2D texture_diffuse1;
uniform sampler2D texture_diffuse2;
uniform sampler2D texture_diffuse3;
uniform sampler2D texture_specular1;
uniform sampler2D texture_specular2;

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir)
{
    vec3 lightDir = normalize(-light.direction);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), /*SHININESS*/0.5f);
    // combine results
    vec3 ambient  = light.ambient;
    vec3 diffuse  = light.diffuse  * diff;
    vec3 specular = light.specular * spec;
    return (ambient + diffuse + specular);
}  

void main()
{
    float gamma = 2.2;

    //vec4 lightless = texture(texture_diffuse1, TexCoords);
    vec4 lightless = vec4(1.0f);
    if (lightless.a < 0.1) {
        discard;
    }

    vec3 norm = normalize(normal);
    vec3 viewDir = normalize(viewPos - FragPos);

    vec4 pregamma = (lightless) * vec4(CalcDirLight(dirLight,norm,viewDir),1.0f) * vec4(vec3(1.0f-1.0f/gl_FragCoord[3]/ 200.0f), 1.0f);

    FragColor = vec4(pow(pregamma.rgb, vec3(1.0/gamma)), pregamma.a);

}