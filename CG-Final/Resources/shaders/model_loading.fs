#version 330 core
out vec4 FragColor;

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;
in vec4 FragPosLightSpace;

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
uniform sampler2D shadowMap;
uniform PointLight pointlight;
uniform vec3 viewPos;

float ShadowCalculation(vec4 fragPosLightSpace) 
{
    //透视除法
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    //将深度纹理变换到[0,1]区间中
    projCoords = projCoords * 0.5 + 0.5;
    //距离光源最近的片段的深度值
    float closestDepth = texture(shadowMap, projCoords.xy).r;
    //当前片段的深度值
    float currentDepth = projCoords.z;
    //通过该片段的法向量和光线方向来确定当前深度与最小深度的差值阈值
    vec3 normal = normalize(Normal);
    vec3 lightDir = normalize(pointlight.position - FragPos);
    float bias = max(0.05 * (1.0 - dot(normal, lightDir)), 0.005);
    float shadow = 0.0;
    vec2 texelSize = 1.0 / textureSize(shadowMap, 0);
    //遍历附近3*3的片段进行采样，并进行平均化
    for (int x = -1; x <= 1; ++x) {
        for (int y = -1; y <= 1; ++y) {
            //计算该片段的深度
            float pcfDepth = texture(shadowMap, projCoords.xy + vec2(x, y) * texelSize).r;
            //若满足条件，则判断处于阴影之中，shadow += 1
            shadow += currentDepth - bias > pcfDepth ? 1.0 : 0.0;
        }
    }
    shadow /= 9.0;

    //若深度值大于1，则置零
    if (projCoords.z > 1.0) 
        shadow = 0.0;

    return shadow;
}

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

	float shadow = ShadowCalculation(FragPosLightSpace);
	vec3 result = ambient + (1 - shadow) * (diffuse + specular);
    // vec3 result = ambient + diffuse + specular;
    FragColor = vec4(result, 1.0);
}