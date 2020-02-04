#shader vertex
#version 430 core
layout (location = 0) in vec4 aPos;
layout (location = 1) in vec3 aNormal;

uniform mat4 u_Model, u_View, u_Projection;

out vec3 normal, FragPos;
void main()
{
    const vec4 vertexPos = u_Model * aPos;
    gl_Position = u_Projection * u_View * vertexPos;
    FragPos = vec3(vertexPos);
    normal = normalize(mat3(transpose(inverse(u_Model)))*aNormal);
}

#shader fragment
#version 430 core
in vec3 normal, FragPos;

out vec4 FragColor;

uniform vec3 u_ObjectColor, u_LightColor, u_LightPos, u_ViewPos;

void main()
{
    const vec3 lightDir = normalize(u_LightPos-FragPos);

    // 0.1 is ambient Strength
    const float ambient =0.1;
    const float diffuse = max(dot(normal, lightDir), 0.0);

    const float specularStrength = 0.8;
    const vec3 viewDir = normalize(u_ViewPos-FragPos);
    const vec3 reflectDir = reflect(-lightDir, normal);
    const float specular = specularStrength * pow(max(dot(viewDir, reflectDir), 0.0), 256);

    const vec3 Color = (ambient+diffuse+specular)*u_ObjectColor*u_LightColor;

    FragColor = vec4(Color, 1.0);
}
