#version 400

layout (location=0) in vec3 vertex_position;
layout (location=1) in vec3 vertex_normal;
layout (location=2) in vec2 vertex_textureCoord;

out vec3 Position;
out vec3 Normal;
out vec2 TextureCoord;

uniform mat4 mvp;
uniform mat3 normalMatrix;
uniform mat4 projectionMatrix;
uniform mat4 modelViewMatrix;

void main()
{
	TextureCoord = vertex_textureCoord;
	Normal = normalize( normalMatrix * vertex_normal );
	Position = vec3( modelViewMatrix * vec4(vertex_position, 1 ) );
	gl_Position = mvp * vec4(vertex_position, 1);
}