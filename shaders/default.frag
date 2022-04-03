#version 330 core
out vec4 FragColor;

in vec3 color;
in vec2 texCoord;
in vec3 Normal;
in vec3 currentPos; 

uniform sampler2D tex0;

uniform vec4 lightColor;
uniform vec3 lightPos;
uniform vec3 camPos;

float ambientAmount = 0.2;
float specularMultiplier = 1;
float specularAmount = 16;

void main()
{
	vec3 normal = normalize(Normal);
	vec3 lightDirection = normalize(lightPos - currentPos);

	float diffuse = max(dot(normal, lightDirection), 0.0);

	vec3 viewDirection = normalize(camPos - currentPos);
	vec3 reflectionDirection = reflect(-lightDirection, normal);
	float currentSpecAmount = pow(max(dot(viewDirection, reflectionDirection), 0.0), specularAmount);
	float specularLight = currentSpecAmount * specularMultiplier;

	FragColor = texture(tex0, texCoord) * lightColor * (diffuse + ambientAmount + specularLight);
}