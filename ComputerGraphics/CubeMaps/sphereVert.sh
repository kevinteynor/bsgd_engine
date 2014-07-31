#version 400

layout (location=0) in vec3 VertexPosition;
layout (location=1) in vec4 MaterialColor;
layout (location=2) in vec3 SurfaceNormal;
layout (location=3) in vec2 UV;
layout (location=4) in vec3 SurfaceNormalTangent;

out vec2 TexCoord;
out vec3 ReflectDir;

uniform mat4 MVP;
uniform mat3 normalMatrix;
uniform mat4 M;
uniform mat4 V;
uniform mat4 P;
uniform vec3 EyePosition;

void main()
{
	vec3 worldPos = vec3(M * vec4(VertexPosition, 1));
	vec3 worldNorm = vec3(M * vec4(SurfaceNormal, 0));
	vec3 worldView = normalize(EyePosition - worldPos);

	ReflectDir = reflect(-worldView, worldNorm);

	TexCoord = UV;
	gl_Position = MVP * vec4(VertexPosition, 1);
}