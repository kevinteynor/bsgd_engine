#version 400

layout (location=0) in vec3 vertex_position;
layout (location=1) in vec4 material_color;
layout (location=2) in vec3 surface_normal;
layout (location=3) in vec2 uv_offset;

out vec3 worldSpaceNormal;
out vec3 vertexWorldPosition;
out vec4 shadowmapCoord;
out vec2 textureCoord;

uniform mat3 modelToWorld_Normal;
uniform mat4 modelToWorld;
uniform mat4 mvp;
uniform mat4 shadowmapMVP;

void main()
{
	worldSpaceNormal = modelToWorld_Normal * surface_normal;
	vertexWorldPosition = vec3(modelToWorld * vec4(vertex_position, 1));

	gl_Position = mvp * vec4(vertex_position, 1);
	shadowmapCoord = shadowmapMVP * vec4(vertex_position, 1);

	textureCoord = uv_offset;
}