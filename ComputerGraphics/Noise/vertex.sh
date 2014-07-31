#version 400

layout (location = 0) in vec3 VertexPosition;
layout (location = 3) in vec2 UV;

out vec2 texCoord;

uniform mat4 MVP;

void main()
{
	texCoord = UV;

	gl_Position = MVP * vec4(VertexPosition, 1);
}