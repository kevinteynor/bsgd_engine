#version 400

layout (location = 0) in vec3 VertexPosition;
layout (location = 1) in vec4 MaterialColor;
layout (location = 2) in vec3 VertexNormal;
layout (location = 3) in vec2 UV;
layout (location = 4) in vec3 VertexNormalTangent;

uniform mat4 MVP;
uniform mat4 shadowmapMVP;

uniform float meltedLevel;

out vec2 texCoord;
out vec3 vertexPos;
out vec3 surfaceNorm;
out vec3 surfaceNormTangent;
out vec3 surfaceNormBitangent;
out vec4 shadowPos;

void main()
{
	vec3 pos = VertexPosition + (VertexNormal * meltedLevel);
	//pos.y = pos.y - (pos.y * meltedLevel);
	gl_Position = MVP * vec4(pos, 1);
	//gl_Position = MVP * vec4(VertexPosition, 1);
	shadowPos = shadowmapMVP * vec4(pos, 1);
	//shadowPos = shadowmapMVP * vec4(VertexPosition, 1);
	texCoord = UV;
	vertexPos = VertexPosition;
	surfaceNorm = normalize(VertexNormal);
	surfaceNormTangent = VertexNormalTangent;
	surfaceNormBitangent = cross(VertexNormal, VertexNormalTangent);
}