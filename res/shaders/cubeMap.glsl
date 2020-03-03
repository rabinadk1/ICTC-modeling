#shader vertex
#version 430 core
layout (location = 0) in vec3 aPos;

out vec3 TexCoords;

uniform mat4 u_Projection, u_View;

void main()
{
    TexCoords = aPos;
    vec4 pos = u_Projection * u_View * vec4(aPos, 1.0);
    gl_Position = pos.xyww;
}

#shader fragment
#version 430 core
out vec4 FragColor;

in vec3 TexCoords;

uniform samplerCube u_CubeMap;

void main()
{
    FragColor = texture(u_CubeMap, TexCoords);
}
