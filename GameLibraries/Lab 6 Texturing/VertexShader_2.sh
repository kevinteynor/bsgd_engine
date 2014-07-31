#version 400

layout (location=0) in vec3 vertex_position;
layout (location=1) in vec4 mat_color;
layout (location=2) in vec3 surface_normal;
layout (location=3) in vec2 uv_offset;

out vec4 color;
out vec3 worldSpaceNormal;
out vec3 vertexWorldPosition;

uniform mat3 modelToWorld_Normal;
uniform mat4 modelToWorld;
uniform mat4 mvp;

void main()
{
	worldSpaceNormal = modelToWorld_Normal * surface_normal;
	vertexWorldPosition = vec3(modelToWorld * vec4(vertex_position, 1));
	color = mat_color;

	gl_Position = mvp * vec4(vertex_position, 1);
}