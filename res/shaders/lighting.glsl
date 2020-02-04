#shader vertex
#version 430 core
layout (location = 0) in vec4 aPos;
layout (location = 1) in vec3 aNormal;

uniform mat4 u_Model, u_View, u_Projection;
uniform vec3 u_ObjectColor, u_LightColor, u_LightPos, u_ViewPos;

out vec3 Color;

void main()
{
    vec4 vertexPos = u_Model * aPos;
    gl_Position = u_Projection * u_View * vertexPos;
    const vec3 normal = normalize(mat3(transpose(inverse(u_Model)))*aNormal);
    const vec3 lightDir = normalize(u_LightPos-vec3(vertexPos));

    // 0.1 is ambient Strength
    const float ambient =0.1;
    const float diffuse = max(dot(normal, lightDir), 0.0);

    const float specularStrength = 0.5;
    const vec3 viewDir = normalize(u_ViewPos-vec3(vertexPos));
    const vec3 reflectDir = reflect(-lightDir, normal);
    const float specular = specularStrength * pow(max(dot(viewDir, reflectDir), 0.0), 256);

    Color = (ambient+diffuse+specular)*u_ObjectColor*u_LightColor;
}

#shader fragment
#version 430 core
in vec3 Color;

out vec4 FragColor;

void main()
{
    FragColor = vec4(Color, 1.0);
}
