#version 400

layout (location=0) in vec3 vertex_position;
layout (location=1) in vec4 mat_color;
out vec4 color;

uniform mat4 mvp;

void main()
{
	color = vec4(0,0,1,1);
	gl_Position = mvp * vec4(vertex_position, 1);
}