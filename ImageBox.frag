#version 330 core
out vec4 FragColor;
  
in vec2 TexCoords;

uniform vec4 Color = {0.0f,0.0f,0.0f,1.0f};
uniform vec4 BackgroundColor = {0.0f,0.0f,0.0f,1.0f};

uniform sampler2D tex;

void main()
{ 
    FragColor = Color * texture(tex,TexCoords);
    if (FragColor.w == 0.0f) {
        
    }
}