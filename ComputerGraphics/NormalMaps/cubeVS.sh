#version 400

layout (location=0) in vec3 vertex_position;
layout (location=1) in vec4 material_color;
layout (location=2) in vec3 surface_normal;
layout (location=3) in vec2 uv_offset;
layout (location=4) in vec3 surface_normal_tangent;

out vec2 textureCoord;

out vec3 vertexPos;
out vec3 surfaceNorm;
out vec3 surfaceNormTangent;
out vec3 surfaceNormBitangent;

uniform mat4 m;
uniform mat4 v;
uniform mat4 p;

uniform vec3 light_position;
uniform vec3 eye_pos;

void main()
{
	textureCoord = uv_offset;
	gl_Position = p * v * m * vec4(vertex_position, 1);

	vertexPos = vertex_position;
	surfaceNorm = surface_normal;
	surfaceNormTangent = surface_normal_tangent;
	surfaceNormBitangent = cross(surface_normal, surface_normal_tangent);
}