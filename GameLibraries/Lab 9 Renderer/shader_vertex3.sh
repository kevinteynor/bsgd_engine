#version 400

layout (location=0) in vec3 vertex_position;
layout (location=1) in vec4 mat_color;
layout (location=2) in vec3 surface_normal;

out vec4 color;

uniform vec3 light_color;
uniform vec3 light_position;
uniform vec3 eye_pos;

uniform mat4 mvp;
uniform mat4 modelToWorld;
uniform mat3 modelToWorld_norm;

void main()
{
	vec3 worldSpaceNormal = modelToWorld_norm * surface_normal;
	vec3 vertexWorldPosition = vec3(modelToWorld * vec4(vertex_position, 1));
	vec3 lightVector = normalize(light_position - vertexWorldPosition);

	// ambient
	vec4 ambientLight = vec4(light_color, 1) * vec4(0.1f, 0.1f, 0.1f, 1);

	// diffuse
	float diffuseBrightness = dot(worldSpaceNormal, lightVector);
	vec4 diffuseLight = vec4(diffuseBrightness, diffuseBrightness, diffuseBrightness, 1) *
		vec4(light_color, 1) * vec4(0.5f,0.5f,0.5f,1.0f);

	// specular
	vec3 eyeVector = normalize(eye_pos - vertexWorldPosition);
	vec3 reflectedLight = reflect( -lightVector, worldSpaceNormal);
	float specBrightness = dot(reflectedLight, eyeVector);
	vec4 specularLight = vec4(0,0,0,4);
	if(specBrightness > 0)
	{
		specBrightness = pow(specBrightness, 10);
		specularLight = vec4(specBrightness, specBrightness, specBrightness, 1.0f) * 
			vec4(light_color, 1) * vec4(0.5f,0.5f,0.5f,1.0f);
	}

	color = ambientLight + diffuseLight + specularLight;
	gl_Position = mvp * vec4(vertex_position, 1);
}