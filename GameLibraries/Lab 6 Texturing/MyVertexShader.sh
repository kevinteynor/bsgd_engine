#version 400

layout (location=0) in vec3 vertex_position;
layout (location=1) in vec4 mat_color;
layout (location=2) in vec3 surface_normal;
layout (location=3) in vec2 uv_offset;
out vec4 color;

// ambient
uniform vec3 ambient_global;
uniform vec3 ambient_material;

// diffuse / specular
uniform vec3 diffuse_material;
uniform vec3 specular_material;

uniform vec3 light_position;
uniform vec3 light_color;

// camera
uniform vec3 eye_pos;

uniform mat3 modelToWorld_Normal;
uniform mat4 modelToWorld;
uniform mat4 mvp;

void main()
{
	vec3 worldSpaceNormal = modelToWorld_Normal * surface_normal;
	vec3 vertexWorldPosition = vec3(modelToWorld * vec4(vertex_position, 1));
	vec3 lightVector = normalize(light_position - vertexWorldPosition);

	// Ambient:
	vec4 ambientLight = vec4(ambient_global, 1) * vec4(ambient_material, 1);

	// Diffuse:	
	float diffuseBrightness = dot(worldSpaceNormal, lightVector);
	vec4 diffuseLight = vec4(diffuseBrightness, diffuseBrightness, diffuseBrightness, 1) * 
			vec4(light_color, 1) * vec4(diffuse_material, 1);

	// Specular:
	vec3 eyeVector = normalize(eye_pos - vertexWorldPosition);
	vec3 reflectedLight = reflect( -lightVector, worldSpaceNormal);
	float specularBrightness = dot(reflectedLight, eyeVector);
	vec4 specularLight = vec4(0,0,0,1);
	if(specularBrightness > 0)
	{
		specularBrightness = pow(specularBrightness, 10);
		specularLight = vec4(specularBrightness, specularBrightness, specularBrightness, 1.0f) * vec4(specular_material, 1.0f);
	}
		 

	color = ambientLight + diffuseLight + specularLight;
	gl_Position = mvp * vec4(vertex_position, 1);
}