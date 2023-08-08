#version 330 core
out vec4 FragColor;

uniform vec3 objectColor;
uniform vec3 lightPos;
uniform vec3 viewPos;

uniform sampler2D texture1;

struct Material {
    sampler2D diffuse;
    sampler2D specular;
    sampler2D emission;
    float shininess;
};

uniform Material material;

struct Light {
    vec3 position;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

uniform Light light;

in vec2 TexCoord;
in vec3 Normal;
in vec3 FragPos;

void main() {
    vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoord));

    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(light.position - FragPos);
    float diff = max(dot(norm, lightDir), 0.0f);
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoord));

    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0f), material.shininess);
    vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoord));

    vec3 emission = texture(material.emission, TexCoord).rbg;

    FragColor = vec4(ambient + diffuse + specular + emission, 1.0f);
    // FragColor = vec4((ambient + diffuse + specular) * objectColor, 1.0f);
    // FragColor = vec4((ambient + diffuse + specular) * vec3(texture(texture1, TexCoord)), 1.0f);
}