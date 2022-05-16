#version 330 core
out vec4 FragColor;

in vec3 currentPos;
in vec3 Normal;
in vec3 color;
in vec2 texCoord;

uniform sampler2D diffuse0;
uniform sampler2D specular0;

uniform vec4 lightColor;
uniform vec3 lightPos;
uniform vec3 camPos;

float ambientAmount = 0.2;
float specularMultiplier = 0.5;
float specularAmount = 8;

//Spotlight values
float outerCone = 0.9;
float innerCone = 0.95;

vec3 directionalLightDir = vec3(1.0, 1.0, 0.0);

vec4 pointLight()
{
	vec3 lightVec = lightPos - currentPos;
	float dist = length(lightVec);
	float a = 3.0;
	float b = 0.7;
	float intensity = 1.0 / (a * dist * dist + b * dist + 1.0);

	vec3 normal = normalize(Normal);
	vec3 lightDirection = normalize(lightVec);

	float diffuse = max(dot(normal, lightDirection), 0.0);

	vec3 viewDirection = normalize(camPos - currentPos);
	vec3 reflectionDirection = reflect(-lightDirection, normal);
	float currentSpecAmount = pow(max(dot(viewDirection, reflectionDirection), 0.0), specularAmount);
	float specularLight = currentSpecAmount * specularMultiplier;
	return (texture(diffuse0, texCoord) * (diffuse * intensity + ambientAmount) + texture(specular0, texCoord).r * specularLight * intensity) * lightColor;
}

vec4 spotLight()
{
	vec3 normal = normalize(Normal);
	vec3 lightDirection = normalize(lightPos - currentPos);

	float diffuse = max(dot(normal, lightDirection), 0.0);

	vec3 viewDirection = normalize(camPos - currentPos);
	vec3 reflectionDirection = reflect(-lightDirection, normal);
	float currentSpecAmount = pow(max(dot(viewDirection, reflectionDirection), 0.0), specularAmount);
	float specularLight = currentSpecAmount * specularMultiplier;

	float angle = dot(vec3(0.0, -1.0, 0.0), -lightDirection);
	float intensity = clamp((angle - outerCone) / (innerCone - outerCone), 0.0, 1.0);

	return (texture(diffuse0, texCoord) * (diffuse * intensity + ambientAmount) + texture(specular0, texCoord).r * specularLight * intensity) * lightColor;
}

vec4 directionalLight()
{
	vec3 normal = normalize(Normal);
	vec3 lightDirection = normalize(directionalLightDir);

	float diffuse = max(dot(normal, lightDirection), 0.0);

	vec3 viewDirection = normalize(camPos - currentPos);
	vec3 reflectionDirection = reflect(-lightDirection, normal);
	float currentSpecAmount = pow(max(dot(viewDirection, reflectionDirection), 0.0), specularAmount);
	float specularLight = currentSpecAmount * specularMultiplier;
	return (texture(diffuse0, texCoord) * (diffuse + ambientAmount) + texture(specular0, texCoord).r * specularLight) * lightColor;
}

void main()
{
	FragColor = directionalLight();
}