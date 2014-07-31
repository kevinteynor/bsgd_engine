#version 400

layout (location=0) in vec3 VertexPosition;		// Position
layout (location=1) in vec3 VertexNormal;		// Normal
layout (location=2) in vec2 VertexTexCoord;		// UV

out vec2 TexCoord;
out vec3 worldSpaceNormal;
out vec3 vertexWorldPosition;

uniform mat3 modelToWorld_Normal;
uniform mat4 modelToWorld;
uniform mat4 mvp;

void main()
{
	worldSpaceNormal = modelToWorld_Normal * VertexNormal;
	vertexWorldPosition = vec3(modelToWorld * vec4(VertexPosition, 1));
	TexCoord = VertexTexCoord;
	gl_Position = mvp * vec4(VertexPosition, 1);
}