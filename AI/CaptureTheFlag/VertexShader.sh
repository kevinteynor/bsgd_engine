#version 400

layout (location=0) in vec3 vertex_position;
layout (location=1) in vec4 mat_color;
layout (location=2) in vec3 surface_normal;
layout (location=3) in vec3 uv_offset;

out vec4 color;

uniform vec3 ambient;
uniform vec3 diffuse;
uniform vec3 light_position;

uniform mat3 modelToWorld_Normal;
uniform mat4 modelToWorld;
uniform mat4 mvp;

void main()
{
	vec3 worldSpaceNormal = modelToWorld_Normal * surface_normal;
	vec3 vertexWorldPosition = vec3(modelToWorld * vec4(vertex_position, 1));
	vec3 lightVector = normalize(light_position - vertexWorldPosition);

	// Ambient:
	vec4 ambientLight = vec4(ambient, 1);

	// Diffuse:
	float diffuseBrightness = dot(worldSpaceNormal, lightVector);
	vec4 diffuseLight = vec4(diffuseBrightness, diffuseBrightness, diffuseBrightness, 1) * vec4(diffuse, 1);

	color = ambientLight + diffuseLight;

	gl_Position = mvp * vec4(vertex_position, 1);
}