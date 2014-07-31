#version 400
#define PI 3.14159265

in vec2 texCoord;
in vec3 vertexPos;
in vec3 surfaceNorm;
in vec3 surfaceNormTangent;
in vec3 surfaceNormBitangent;
in vec4 shadowPos;

out vec4 FragColor;

uniform mat4 M;
uniform mat4 V;
uniform mat4 P;
uniform vec3 LightPos;
uniform vec3 EyePos;

uniform sampler2D normalMap;
uniform sampler2D textureMap;
uniform sampler2D shadowMap;
uniform sampler2D noiseMap;

uniform vec4 color;
uniform float meltedLevel;

uniform int UseLighting;
uniform int UseNormalMap;
uniform int UseShadows;

void main()
{
	vec4 textureColor = vec4(0,0,0,0);
	vec3 normal = surfaceNorm;
	vec3 lightVector = normalize(LightPos - vertexPos);
	vec3 eyeVector = normalize(EyePos - vertexPos);

	vec4 ambientLight = vec4(0.4f, 0.4f, 0.4f, 1);
	vec4 diffuseLight = vec4(0,0,0,1);
	vec4 specularLight = vec4(0,0,0,0);

	if(UseNormalMap == 1)
	{
		vec3 vertexWorldPos = (M * vec4(vertexPos, 1)).xyz;
		vec3 vertexViewPos = (V * M * vec4(vertexPos, 1)).xyz;
		
		vec3 eyeViewVector = vec3(0,0,0) - vertexViewPos;
		vec3 lightViewPos = (V * vec4(LightPos, 1)).xyz;
		vec3 lightViewVector = lightViewPos + eyeViewVector;
		
		mat3 modelToView3 = mat3(V * M);
		vec3 viewNormal = normalize(modelToView3 * surfaceNorm);
		vec3 viewTangent = normalize(modelToView3 * surfaceNormTangent);
		vec3 viewBitangent = normalize(modelToView3 * surfaceNormBitangent);
		mat3 tangentTransform = transpose(mat3(viewTangent, viewBitangent, viewNormal));
		
		vec3 lightVectorTangent = tangentTransform * normalize(lightViewVector);
		vec3 eyeVectorTangent = tangentTransform * normalize(eyeViewVector);
		
		normal = (texture(normalMap, vec2(texCoord.x, -texCoord.y)).rgb * 2.0 - 1.0);

		lightVector = lightVectorTangent;
		eyeVector = eyeVectorTangent;
	}
	vec4 lightColor = vec4(0,0,0,1);
	if(UseLighting == 1)
	{
		float diffuseBrightness = max(dot(lightVector, normal), 0.0f);
		diffuseLight = vec4(diffuseBrightness, diffuseBrightness, diffuseBrightness, 1);
	
		vec3 r = reflect(-lightVector, normal);
		float specularBrightness = dot(r, eyeVector);
		specularLight = vec4(0,0,0,0);
		if(specularBrightness > 0)
		{
			specularBrightness = pow(specularBrightness, 10);
			specularLight = vec4(specularBrightness, specularBrightness, specularBrightness, 1.0);
		}

		lightColor = ambientLight + (diffuseLight * 0.25) + (specularLight * 0.25);
	}

	//	if(texture(noiseMap, texCoord).a < meltedLevel)
	//	{
	//		discard;
	//	}

	if(UseShadows == 1)
	{
		if(texture(shadowMap, shadowPos.xy).z < shadowPos.z - 0.005)
		{
			lightColor = ambientLight;
		}
	}

	FragColor = color * lightColor;
}