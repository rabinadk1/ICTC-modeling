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

// struct for directional light(like the sun)
struct DirLight {
    vec3 direction;

    // light properties
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

// struct for point lights
struct PointLight
{
    vec3 position;

    // constants for attenuation
    float constant;
    float linear;
    float quadratic;

    // light properties
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};
#define NR_POINT_LIGHTS 2  


uniform Material u_Material;
uniform DirLight u_DirLight;
uniform PointLight u_PointLight[NR_POINT_LIGHTS];
uniform vec3 u_ViewPos;

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir);
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);

void main()
{
    vec3 viewDir = normalize(u_ViewPos - FragPos);

    // directional lighting
    vec3 result = CalcDirLight(u_DirLight, normal, viewDir);
    for(int i = 0; i < NR_POINT_LIGHTS; i++)
        result += CalcPointLight(u_PointLight[i], normal, FragPos, viewDir);

    FragColor = vec4(result, 1.0);
}

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    /*
    We use Phong Shading Model to calculate the 
    illumination of the fragments
    */

    // for material diffuse property
    vec4 col_diffuse_1 = texture(u_Material.diffuse1, TexCoords);
    vec4 col_diffuse_2 = texture(u_Material.diffuse2, TexCoords);
    vec4 material_diffuse =  0.5 * col_diffuse_1 + 0.5 * col_diffuse_2;

    // for material specular property
    vec4 col_specular_1 = texture(u_Material.specular1, TexCoords);
    vec4 col_specular_2 = texture(u_Material.specular2, TexCoords);
    vec4 material_specular = col_specular_1 * col_specular_2;

    vec3 lightDir = normalize(light.position - fragPos);
    // diffuse shading constant
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading constant
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), u_Material.shininess);

    // for light attenuation
    float distance = length(light.position - FragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance +
                                light.quadratic * (distance * distance));   

    // combine ambient, diffuse and specular shading (phong model)
    // here we multiply these two because as of now we donot
    // have any texture in our model
    vec3 ambient = light.ambient * material_diffuse.rgb;
    vec3 diffuse = light.diffuse * diff * material_diffuse.rgb;
    vec3 specular = light.specular * spec * material_specular.rgb;

    // return the combined effect of all shading
    // for point lighting with appropriate attenuation
    return (ambient*attenuation + diffuse*attenuation + specular*attenuation);

}

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir)
{
    /*
    We use Phong Shading Model to calculate the 
    illumination of the fragments
    */

    // for material diffuse property
    vec4 col_diffuse_1 = texture(u_Material.diffuse1, TexCoords);
    vec4 col_diffuse_2 = texture(u_Material.diffuse2, TexCoords);
    vec4 material_diffuse =  0.5 * col_diffuse_1 + 0.5 * col_diffuse_2;

    // for material specular property
    vec4 col_specular_1 = texture(u_Material.specular1, TexCoords);
    vec4 col_specular_2 = texture(u_Material.specular2, TexCoords);
    vec4 material_specular = col_specular_1 * col_specular_2;

    vec3 lightDir = normalize(-light.direction);
    // diffuse shading constant
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading constant
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), u_Material.shininess);

    // combine ambient, diffuse and specular shading (phong model)

    vec3 ambient = light.ambient * material_diffuse.rgb;
    vec3 diffuse = light.diffuse * diff * material_diffuse.rgb;
    vec3 specular = light.specular * spec * material_specular.rgb;

    // return the combined effect of all shading
    // for directional lighting
    return (ambient + diffuse + specular);
}
