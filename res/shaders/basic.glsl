#shader vertex
#version 430 core
layout (location = 0) in vec4 position;
void main()
{
   gl_Position = position;
};

#shader fragment
#version 430 core
out vec4 FragColor;
uniform vec4 u_Color;

void main()
{
    FragColor = u_Color;
};
