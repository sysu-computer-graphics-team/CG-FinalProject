#version 330 core
out vec4 FragColor;

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;

struct PointLight 
{
	vec3 position;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	float constant;
	float linear;
	float quadratic;
};

uniform sampler2D texture_diffuse1;
uniform sampler2D texture_specular1;
uniform PointLight pointlight;
uniform vec3 viewPos;

void main()
{    
	// ambient
	vec3 ambient = pointlight.ambient * texture(texture_diffuse1, TexCoords).rgb;

	// diffuse
	vec3 norm = normalize(Normal);
	vec3 lightDir = normalize(pointlight.position - FragPos);
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = pointlight.diffuse * diff * texture(texture_diffuse1, TexCoords).rgb;

	// specular
	vec3 viewDir = normalize(viewPos - FragPos);
	vec3 reflectDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
	vec3 specular = pointlight.specular * spec * texture(texture_specular1, TexCoords).rgb;

	// attenuation
	float distance = length(pointlight.position - FragPos);
	float attenuation = 1.0 / (pointlight.constant + pointlight.linear * distance + pointlight.quadratic * distance * distance);

	ambient *= attenuation;
	diffuse *= attenuation;
	specular *= attenuation;

	vec3 result = ambient + diffuse + specular;
    FragColor = vec4(result, 1.0);
}