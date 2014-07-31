#version 400

in vec3 worldSpaceNormal;
in vec3 vertexWorldPosition;
in vec4 shadowmapCoord;
in vec2 textureCoord;

out vec4 FragColor;

uniform vec3 light_position;
uniform vec3 eye_pos;
uniform sampler2D shadowMap;

void main()
{
	vec3 lightVector = normalize(light_position - vertexWorldPosition);

	vec4 ambientLight = vec4(0.1f, 0.1f, 0.1f, 1);
	vec4 diffuseLight = vec4(0,0,0,0);
	vec4 specularLight = vec4(0,0,0,0);

	float bias = 0.005f;
	float visibility = 1.0f;
	if(texture(shadowMap, shadowmapCoord.xy).z < shadowmapCoord.z - bias)
	{
		visibility = 0;
	}

	float diffuseBrightness = dot(worldSpaceNormal, lightVector);
	diffuseLight = vec4(diffuseBrightness, diffuseBrightness, diffuseBrightness, 1);

	vec3 eyeVector = normalize(eye_pos - vertexWorldPosition);
	vec3 reflectedLight = reflect( -lightVector, worldSpaceNormal);
	float specularBrightness = dot(reflectedLight, eyeVector);
	if(specularBrightness > 0)
	{
		specularBrightness = pow(specularBrightness, 10);
		specularLight = vec4(specularBrightness, specularBrightness, specularBrightness, 1.0f);
	}

	FragColor = ambientLight + (diffuseLight * visibility) + (specularLight * visibility);
}