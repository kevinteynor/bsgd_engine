#version 400

layout (location=0) in vec3 vertex_position;
layout (location=1) in vec4 material_color;
layout (location=2) in vec3 surface_normal;
layout (location=3) in vec2 uv_offset;

out vec3 vertexWorldPosition;
out vec2 textureCoord;

uniform mat4 modelToWorld;
uniform mat4 mvp;

void main()
{
	vertexWorldPosition = vec3(modelToWorld * vec4(vertex_position, 1));
	textureCoord = uv_offset;
	gl_Position = mvp * vec4(vertex_position, 1);
}