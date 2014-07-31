#version 400
#define PI 3.14159265

out vec4 FragColor;

in vec2 texCoord;

uniform sampler2D NoiseTex;
uniform vec4 color1 = vec4 (0.3, 0.3, 0.3, 1.0);
uniform vec4 color2 = vec4 (1.0, 1.0, 1.0, 1.0);

void main()
{
	vec4 noise = texture(NoiseTex, texCoord);

	float t = (cos(noise.g * PI) + 1.0) / 2.0;

	vec4 color = mix(color1, color2, t);
	
	FragColor = color;
}