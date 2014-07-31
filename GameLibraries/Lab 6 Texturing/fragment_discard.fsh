#version 400

in vec2 TexCoord;
in vec4 LightColor;
out vec4 OutColor;

uniform sampler2D discardTex;

void main()
{
	vec4 texColor = texture(discardTex, TexCoord);

	if(texColor.a < 0.15)
		discard;

	OutColor = texColor * LightColor;
}