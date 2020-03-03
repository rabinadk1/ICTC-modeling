#shader vertex
#version 430 core
layout (location = 0) in vec4 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

uniform mat4 u_Model, u_View, u_Projection;

out vec3 normal, FragPos;
out vec2 TexCoords;
void main()
{
    const vec4 vertexPos = u_Model * aPos;
    gl_Position = u_Projection * u_View * vertexPos;
    FragPos = vec3(vertexPos);
    normal = normalize(mat3(transpose(inverse(u_Model)))*aNormal);
    TexCoords = aTexCoords;
}

#shader fragment
#version 430 core
in vec3 normal, FragPos;
in vec2 TexCoords;

out vec4 FragColor;

struct Material {
    sampler2D diffuse1;
    sampler2D diffuse2;
    sampler2D specular1;
    sampler2D specular2;
    float shininess;
};

struct Light {
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    // attenuation variables
    float constant;
    float linear;
    float quadratic;

};


uniform Material u_Material;
uniform Light u_Light;
uniform vec3 u_ViewPos;

void main()
{
    const vec4 col_diffuse_1 = texture(u_Material.diffuse1, TexCoords);
    const vec4 col_diffuse_2 = texture(u_Material.diffuse2, TexCoords);
    const vec4 material_diffuse =  mix(col_diffuse_1, col_diffuse_2, 0.5);
    const vec3 ambient = u_Light.ambient * material_diffuse.rgb;

    const vec3 lightDir = normalize(u_Light.position-FragPos);
    const float diff = max(dot(normal, lightDir), 0.0);
    const vec3 diffuse = diff * u_Light.diffuse * material_diffuse.rgb;

    const vec4 col_specular_1 = texture(u_Material.specular1, TexCoords);
    const vec4 col_specular_2 = texture(u_Material.specular2, TexCoords);
    const vec4 material_specular = col_specular_1 * col_specular_2;

    const vec3 viewDir = normalize(u_ViewPos-FragPos);
    const vec3 reflectDir = reflect(-lightDir, normal);
    const float spec = pow(max(dot(viewDir, reflectDir), 0.0), u_Material.shininess);
    const vec3 specular = spec * u_Light.specular * material_specular.rgb;

    // for light attenuation
    const float distance = length(u_Light.position - FragPos);
    const float attenuation = 1.0 / (u_Light.constant + u_Light.linear * distance +
                                u_Light.quadratic * (distance * distance));

    const vec3 result = ((ambient*attenuation)+(diffuse*attenuation)+(specular*attenuation));
    FragColor = vec4(result, col_diffuse_2.w);
}
