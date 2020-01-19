#shader vertex
#version 430 core
layout (location = 0) in vec4 aPos;
layout (location = 1) in vec3 aNormal;

uniform mat4 u_Model, u_View, u_Projection;
uniform vec3 u_ObjectColor, u_LightColor, u_LightPos;

out vec3 Color;

void main()
{
    vec4 vertexPos = u_Model * aPos;
    gl_Position = u_Projection * u_View * vertexPos;
    const vec3 normal = normalize(aNormal);
    const vec3 lightDir = normalize(u_LightPos-vec3(vertexPos));

    const float ambient =0.1;
    const float diffuse = max(dot(normal, lightDir), 0.0);

    Color = (ambient+diffuse)*u_ObjectColor*u_LightColor;
}

#shader fragment
#version 430 core
in vec3 Color;

out vec4 FragColor;

void main()
{
    // 0.1 is ambient Strength
    FragColor = vec4(Color, 1.0);
}
