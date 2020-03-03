#shader vertex
#version 430 core
layout (location = 0) in vec3 aPos;

out vec3 TexCoord;
uniform mat4 u_View, u_Projection;

void main()
{
   gl_Position = u_Projection*u_View*vec4(aPos, 1.0);
   TexCoord = aPos;
};

#shader fragment
#version 430 core
in vec3 TexCoord;

uniform samplerCube u_CubeMap;

out vec4 FragColor;
void main()
{
    FragColor = texture(u_CubeMap, TexCoord);
};
