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

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};

struct Light {
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};


uniform Material u_Material;
uniform Light u_Light;
uniform vec3 u_LightColor, u_LightPos, u_ViewPos;

void main()
{
    const vec3 ambient = u_Light.ambient * u_Material.ambient;

    const vec3 lightDir = normalize(u_Light.position-FragPos);
    const vec3 diffuse = u_Light.diffuse * (max(dot(normal, lightDir), 0.0) * u_Material.diffuse);

    const vec3 viewDir = normalize(u_ViewPos-FragPos);
    const vec3 reflectDir = reflect(-lightDir, normal);
    const vec3 specular = u_Light.specular * (u_Material.specular * pow(max(dot(viewDir, reflectDir), 0.0), u_Material.shininess));

    const vec3 result = (ambient+diffuse+specular);
    FragColor = vec4(result, 1.0);
}
