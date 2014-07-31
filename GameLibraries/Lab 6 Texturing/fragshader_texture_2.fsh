#version 400

in vec2 TexCoord;
in vec3 worldSpaceNormal;
in vec3 vertexWorldPosition;

out vec4 OutColor;

uniform vec3 ambient_global;
uniform vec3 ambient_material;
uniform vec3 diffuse_material;
uniform vec3 specular_material;
uniform vec3 light_position;
uniform vec3 light_color;
uniform vec3 eye_pos;

uniform sampler2D texture1;
uniform sampler2D slime1;

void main()
{
	vec3 lightVector = normalize(light_position - vertexWorldPosition);

	// ambient:
	vec4 ambientLight = vec4(ambient_global, 1) * vec4(ambient_material, 1);

	// diffuse:
	float diffuseBrightness = dot(worldSpaceNormal, lightVector);
	vec4 diffuseLight = vec4(diffuseBrightness, diffuseBrightness, diffuseBrightness, 1) *
		vec4(light_color, 1) *
		vec4(diffuse_material, 1);

	// specular:
	vec3 eyeVector = normalize(eye_pos - vertexWorldPosition);
	vec3 reflectedLight = reflect( -lightVector, worldSpaceNormal);
	float specularBrightness = dot(reflectedLight, eyeVector);
	
	vec4 specularLight = vec4(0,0,0,1);
	if(specularBrightness > 0)
	{
		specularBrightness = pow(specularBrightness, 10);
		specularLight = vec4(specularBrightness, specularBrightness, specularBrightness, 1.0f) * vec4(specular_material, 1.0f);
	}

	vec4 lightColor = ambientLight + diffuseLight + specularLight;
	vec4 texColor = texture(texture1, TexCoord);
	vec4 slimeColor = texture(slime1, TexCoord);
	vec4 matColor = mix(texColor, slimeColor, slimeColor.a);

	OutColor =	matColor * lightColor;
}