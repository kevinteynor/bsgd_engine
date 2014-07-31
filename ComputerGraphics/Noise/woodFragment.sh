#version 400

out vec4 FragColor;

in vec2 texCoord;

uniform sampler2D NoiseTex;
uniform vec3 color1;
uniform vec3 color2;
uniform mat4 Slice;

uniform float LowThreshold;

void main()
{
	vec4 cyl = Slice * vec4(texCoord, 0.0, 1.0);
	
	float dist = length(cyl.xz);
	
	vec4 noise = texture(NoiseTex, texCoord);
	dist += noise.b;

	float t = 1.0 - abs(fract(dist) * 2.0 - 1.0);
	t = smoothstep(0.2, 0.5, t);
	vec4 color = mix(vec4(color1, 1), vec4(color2, 1), t);
	
	if(noise.a < LowThreshold - 0.06f)
	{
		discard;		
	}
	else if(noise.a < LowThreshold)
	{
		FragColor = mix(vec4(0.2,0.2,0.2,1), vec4(0.15,0.15,0.15,1), t);
	}
	else if(noise.a < LowThreshold + 0.015)
	{
		FragColor = vec4(1,0,0,1);
	}
	else if(noise.a < LowThreshold + 0.025)
	{
		FragColor = vec4(1.0,0.55,0,1);
	}
	else
	{		
		FragColor = vec4(color.rgb, 1.0);
	}
}