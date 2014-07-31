#version 400

in vec2 TexCoord;
in vec3 ReflectDir;

out vec4 FragColor;

uniform float Reflectivity;
uniform samplerCube CubeMapTex;

void main()
{
	vec4 cubeMapColor = texture(CubeMapTex, ReflectDir);
	
	vec4 materialColor = vec4(0);
	FragColor = mix(materialColor, cubeMapColor, Reflectivity);
}