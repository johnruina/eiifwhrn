#version 330 core
out vec4 FragColor;
  
in vec2 TexCoords;

uniform sampler2D screenTexture;
uniform float brightness;
uniform float colorContrast;

void main()
{ 
    //FragColor =vec4(texture(screenTexture, TexCoords));
    vec3 originalcolor = texture(screenTexture, TexCoords).rgb;
    vec3 finalcolor = originalcolor;

    vec3 contrast = vec3(1.0f) - finalcolor;

    finalcolor = mix(finalcolor,vec3(1.0f) - finalcolor,colorContrast);
    

    FragColor = vec4(finalcolor,1.0f);
    //FragColor = vec4(ORIGINALCOLOR,1.0f);
}