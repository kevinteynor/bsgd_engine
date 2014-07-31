#version 400

in vec3 ReflectDir;

out vec4 FragColor;

uniform samplerCube CubeMapTex;

void main()
{
	FragColor = texture(CubeMapTex, ReflectDir);
}