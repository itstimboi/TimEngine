#version 460 core

// Outputs colors in RGBA
out vec4 FragColor;

// Imports the current position from the Vertex Shader
in vec3 crntPos;
// Imports the normal from the Vertex Shader
in vec3 Normal;
// Imports the color from the Vertex Shader
in vec3 color;
// Imports the texture coordinates from the Vertex Shader
in vec2 texCoord;

// Gets the Texture Unit from the main function
uniform sampler2D diffuse0;
// Gets the color of the light from the main function
uniform vec4 lightColor;
// Gets the position of the light from the main function
uniform vec3 lightPos;
// Gets the position of the camera from the main function
uniform vec3 camPos;

uniform bool usingLights;

struct PointLight {
    vec3 position;
    float constant;
    float linear;
    float quadratic;
    vec3 ambient;
    vec3 diffuse;
    float specular;
};

#define NR_POINT_LIGHTS 16
uniform PointLight pointLights[NR_POINT_LIGHTS];

// vec4 pointLight()
// {
// 	// used in two variables so I calculate it here to not have to do it twice
// 	vec3 lightVec = position - crntPos;

// 	// intensity of light with respect to distance
// 	float dist = length(lightVec);
// 	float a = 3.0;
// 	float b = 0.7;
// 	float inten = 1.0f / (a * dist * dist + b * dist + 1.0f);

// 	// ambient lighting
// 	ambient = 0.20f;

// 	// diffuse lighting
// 	vec3 normal = normalize(Normal);
// 	vec3 lightDirection = normalize(lightPos - crntPos);
// 	float diffuse = max(dot(normal, lightDirection), 0.0f);

// 	// // specular lighting
// 	// float specularLight = 0.50f;
// 	// vec3 viewDirection = normalize(camPos - crntPos);
// 	// vec3 reflectionDirection = reflect(-lightDirection, normal);
// 	// float specAmount = pow(max(dot(viewDirection, reflectionDirection), 0.0f), 8);
// 	// specular = specAmount * specularLight;

// 	return (texture(diffuse0, texCoord) * (diffuse * inten + ambient)) * lightColor;
// }

vec4 pointLight(PointLight light)
{
    vec3 lightVec = light.position - crntPos;

    float dist = length(lightVec);

    float inten = 1.0f /
        (light.constant +
         light.linear * dist +
         light.quadratic * dist * dist);

    vec3 normal = normalize(Normal);

    vec3 lightDirection =
        normalize(light.position - crntPos);

    float diffuse =
        max(dot(normal, lightDirection), 0.0f);

    vec4 texColor =
        texture(diffuse0, texCoord);

    vec3 lighting =
        light.ambient +
        light.diffuse * diffuse * inten;

    return vec4(
        texColor.rgb * lighting,
        texColor.a
    );
}

vec4 direcLight()
{
	// ambient lighting
	float ambient = 0.20f;

	// diffuse lighting
	vec3 normal = normalize(Normal);
	vec3 lightDirection = normalize(vec3(1.0f, 1.0f, 0.0f));
	float diffuse = max(dot(normal, lightDirection), 0.0f);

	// specular lighting
	float specularLight = 0.50f;
	vec3 viewDirection = normalize(camPos - crntPos);
	vec3 reflectionDirection = reflect(-lightDirection, normal);
	float specAmount = pow(max(dot(viewDirection, reflectionDirection), 0.0f), 16);
	float specular = specAmount * specularLight;

	return (texture(diffuse0, texCoord) * (diffuse + ambient)) * lightColor;
}

uniform vec3 flashLightPos;
uniform vec3 flashLightDirection;

uniform float flashLightInnerCone;
uniform float flashLightOuterCone;

uniform bool flashLightEnabled;

struct SpotLight
{
    vec3 position;
    vec3 direction;

    float constant;
    float linear;
    float quadratic;

    vec3 ambient;
    vec3 diffuse;

    float innerCone;
    float outerCone;
};

#define NR_SPOT_LIGHTS 16

uniform SpotLight spotLights[NR_SPOT_LIGHTS];

vec4 spotLight(SpotLight light)
{
    vec3 lightVec = light.position - crntPos;

    float dist = length(lightVec);

    float inten =
        1.0f /
        (
            light.constant +
            light.linear * dist +
            light.quadratic * dist * dist
        );

    vec3 normal = normalize(Normal);

    vec3 lightDirection =
        normalize(light.position - crntPos);

    float diffuse =
        max(
            dot(normal, lightDirection),
            0.0f
        );

    // --------------------------------------------------------
    // Spotlight cone
    // --------------------------------------------------------

    float angle =
        dot(
            normalize(-light.direction),
            normalize(-lightVec)
        );

    float coneIntensity =
        clamp(
            (angle - light.outerCone) /
            (light.innerCone - light.outerCone),
            0.0f,
            1.0f
        );

    // --------------------------------------------------------
    // Texture
    // --------------------------------------------------------

    vec4 texColor =
        texture(diffuse0, texCoord);

    // --------------------------------------------------------
    // Lighting
    // --------------------------------------------------------

    vec3 lighting =
        light.diffuse *
        diffuse *
        inten *
        coneIntensity;

    return vec4(
        texColor.rgb * lighting,
        texColor.a
    );
}

// {
// 	// controls how big the area that is lit up is
// 	float outerCone = 0.90f;
// 	float innerCone = 0.95f;

// 	// ambient lighting
// 	float ambient = 0.20f;

// 	// diffuse lighting
// 	vec3 normal = normalize(Normal);
// 	vec3 lightDirection = normalize(lightPos - crntPos);
// 	float diffuse = max(dot(normal, lightDirection), 0.0f);

// 	// specular lighting
// 	float specularLight = 0.50f;
// 	vec3 viewDirection = normalize(camPos - crntPos);
// 	vec3 reflectionDirection = reflect(-lightDirection, normal);
// 	float specAmount = pow(max(dot(viewDirection, reflectionDirection), 0.0f), 16);
// 	float specular = specAmount * specularLight;

// 	// calculates the intensity of the crntPos based on its angle to the center of the light cone
// 	float angle = dot(vec3(0.0f, -1.0f, 0.0f), -lightDirection);
// 	float inten = clamp((angle - outerCone) / (innerCone - outerCone), 0.0f, 1.0f);

// 	if (texture(diffuse0, texCoord).a < 0.1)
// 		discard;

// 	return (texture(diffuse0, texCoord) * (diffuse * inten + ambient)) * lightColor;
// }

void main()
{
    vec4 texColor = texture(diffuse0, texCoord);

    if (!usingLights)
    {
        FragColor = texColor;
        return;
    }

    vec3 result =
        texColor.rgb * 0.20f;

    // Point lights
    for (int i = 0; i < NR_POINT_LIGHTS; i++)
    {
        result += pointLight(pointLights[i]).rgb;
    }

    // Spot lights
    for (int i = 0; i < NR_SPOT_LIGHTS; i++)
    {
        result += spotLight(spotLights[i]).rgb;
    }

    FragColor =
        vec4(result, texColor.a) *
        lightColor;
}