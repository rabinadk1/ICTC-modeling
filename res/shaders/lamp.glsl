#shader vertex
#version 430 core
layout (location = 0) in vec4 aPos;

// Used here because we have normal in postions too
layout (location = 1) in vec3 aNormal;

uniform mat4 u_Model, u_View, u_Projection;

void main()
{
    gl_Position = u_Projection * u_View * u_Model * aPos;
}

#shader fragment
#version 430 core
out vec4 FragColor;

void main()
{
    FragColor = vec4(1.0); // set all 4 vector values to 1.0
}
