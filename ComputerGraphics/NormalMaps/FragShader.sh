#version 400

in vec3 vertexWorldPosition;
in vec2 textureCoord;
in mat4 mtw;

out vec4 FragColor;

uniform vec3 light_position;
uniform vec3 eye_pos;

uniform sampler2D normalMap;
uniform float specVal;
uniform float diffVal;

void main()
{
	vec3 lightVector = normalize(light_position - vertexWorldPosition);
	vec3 normal = normalize(texture(normalMap, textureCoord).rgb * 2.0 - 1.0);

	//vec4 ambientLight = vec4(0.1f, 0.1f, 0.1f, 1);
	vec4 ambientLight = vec4(normal, 1) * 0.25f;

	float diffuseBrightness = dot(lightVector, normal);
	vec4 diffuseLight = vec4(diffuseBrightness, diffuseBrightness, diffuseBrightness, 1);

	vec3 eyeVector = normalize(eye_pos - vertexWorldPosition);
	vec3 r = reflect( -lightVector, normal);
	float specularBrightness = dot(r, eyeVector);
	vec4 specularLight = vec4(0,0,0,0);
	if(specularBrightness > 0)
	{
		specularBrightness = pow(specularBrightness, 10);
		specularLight = vec4(specularBrightness, specularBrightness, specularBrightness, 1.0f);
	}

	FragColor = ambientLight + (diffuseLight * diffVal) + (specularLight * specVal);
}