#version 400

in vec2 textureCoord;

in vec3 vertexPos;
in vec3 surfaceNorm;
in vec3 surfaceNormTangent;
in vec3 surfaceNormBitangent;

out vec4 FragColor;

uniform sampler2D normalMap;
uniform float specVal;
uniform float diffVal;

uniform vec3 light_position;
uniform vec3 eye_pos;

uniform mat4 m;
uniform mat4 v;
uniform mat4 p;

void main()
{

	vec3 vertexWorldPosition = (m * vec4(vertexPos,1)).xyz;
	vec3 vertexPosition_View = (v * m * vec4(vertexPos,1)).xyz;

	vec3 eyeVector_view = vec3(0,0,0) - vertexPosition_View;
	vec3 lightPos_view = (v * vec4(light_position, 1)).xyz;
	vec3 lightVector_view = lightPos_view + eyeVector_view;
	
	mat3 modelToView3 = mat3(v * m);
	vec3 normal_view = normalize(modelToView3 * surfaceNorm);
	vec3 tangent_view = normalize(modelToView3 * surfaceNormTangent);
	vec3 bitangent_view = normalize(modelToView3 * surfaceNormBitangent);
	mat3 tangentTransform = transpose(mat3(tangent_view, bitangent_view, normal_view));

	vec3 lightVector_tangent = tangentTransform * normalize(lightVector_view);
	vec3 eyeVector_tangent = tangentTransform * normalize(eyeVector_view);
	
	vec3 normal = (texture(normalMap, vec2(textureCoord.x, -textureCoord.y)).rgb * 2.0 - 1.0);

	vec3 lightVector = lightVector_tangent;
	vec3 eyeVector = eyeVector_tangent;

	vec4 ambientLight = vec4(0.1f, 0.1f, 0.1f, 1);   //vec4 ambientLight = vec4(normal, 1) * 0.25f;

	float diffuseBrightness = max(dot(lightVector, normal), 0.0f);
	vec4 diffuseLight = vec4(diffuseBrightness, diffuseBrightness, diffuseBrightness, 1);

	vec3 r = reflect( -lightVector, normal);
	float specularBrightness = dot(r, eyeVector);
	vec4 specularLight = vec4(0,0,0,0);
	if(specularBrightness > 0)
	{
		specularBrightness = pow(specularBrightness, 10);
		specularLight = vec4(specularBrightness, specularBrightness, specularBrightness, 1.0f);
	}

	FragColor = ambientLight + (diffuseLight * diffVal) + (specularLight * specVal);
}