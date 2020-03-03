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

uniform Material u_Material;
uniform DirLight u_DirLight;
uniform vec3 u_ViewPos;

vec4 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir);

void main()
{
    vec3 viewDir = normalize(u_ViewPos - FragPos);

    // directional lighting
    vec4 result = CalcDirLight(u_DirLight, normal, viewDir);

    FragColor = result;
}


vec4 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir)
{
    /*
    We use Phong Shading Model to calculate the 
    illumination of the fragments
    */

    // for material diffuse property
    const vec4 col_diffuse_1 = texture(u_Material.diffuse1, TexCoords);
    const vec4 col_diffuse_2 = texture(u_Material.diffuse2, TexCoords);
    const vec4 material_diffuse =  mix(col_diffuse_1, col_diffuse_2, 0.5);

    // for material specular property
    const vec4 col_specular_1 = texture(u_Material.specular1, TexCoords);
    const vec4 col_specular_2 = texture(u_Material.specular2, TexCoords);
    const vec4 material_specular = col_specular_1 * col_specular_2;

    const vec3 lightDir = normalize(-light.direction);
    // diffuse shading constant
    const float diff = max(dot(normal, lightDir), 0.0);
    // specular shading constant
    const vec3 reflectDir = reflect(-lightDir, normal);
    const float spec = pow(max(dot(viewDir, reflectDir), 0.0), u_Material.shininess);

    // combine ambient, diffuse and specular shading (phong model)

    const vec3 ambient = light.ambient * material_diffuse.rgb;
    const vec3 diffuse = light.diffuse * diff * material_diffuse.rgb;
    const vec3 specular = light.specular * spec * material_specular.rgb;

    // return the combined effect of all shading
    // for directional lighting
    return vec4((ambient + diffuse + specular), material_diffuse.w);
}
