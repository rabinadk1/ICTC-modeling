#shader vertex
#version 430 core
layout (location = 0) in vec4 position;
layout (location = 1) in vec2 aTexCord;

out vec2 TexCord;
uniform mat4 u_MVP;

void main()
{
    // gl_Position = position;
   gl_Position = u_MVP * position;
   TexCord = aTexCord;
};

#shader fragment
#version 430 core
in vec2 TexCord;

uniform sampler2D u_Texture;

out vec4 FragColor;
void main()
{
    FragColor = texture(u_Texture, TexCord);
};
