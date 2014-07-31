#version 400

in vec2 TexCoord;
in vec4 LightColor;
out vec4 OutColor;

uniform sampler2D texture1;
uniform sampler2D slime1;

void main()
{
	vec4 texColor = texture(texture1, TexCoord);
	vec4 slimeColor = texture(slime1, TexCoord);
	vec4 matColor = mix(texColor, slimeColor, slimeColor.a);

	OutColor = matColor * LightColor;
}