#version 410 core

in vec3 fNormal;
in vec4 fPosEye;
in vec2 fTexCoords;
in vec4 fragPosLightSpace;
in vec3 fragWorldPos;

out vec4 fColor;

//lighting
uniform	vec3 lightDir;
uniform	vec3 lightColor;

//texture
uniform sampler2D diffuseTexture;
uniform sampler2D specularTexture;
uniform sampler2D shadowMap;
uniform bool fogEnabled;

//security lamp
uniform vec3 pointLightPosition1;
uniform vec3 pointLightColor1;

uniform float constant1;
uniform float linear1;
uniform float quadratic1;
uniform float intensity1;

//street lamp
uniform vec3 pointLightPosition2;
uniform vec3 pointLightColor2;

uniform float constant2;
uniform float linear2;
uniform float quadratic2;
uniform float intensity2;

//siren light
uniform vec3 pointLightPosition3;
uniform vec3 pointLightColor3;

uniform float constant3;
uniform float linear3;
uniform float quadratic3;
uniform float intensity3;

//cat light
uniform vec3 pointLightPosition4;
uniform vec3 pointLightColor4;
uniform vec3 spotLightDirection;
uniform float constant4;
uniform float linear4;
uniform float quadratic4;
uniform float intensity4;
uniform float spotLightCutOff;

//headlight
uniform vec3 pointLightPosition5;
uniform vec3 pointLightColor5;
uniform vec3 spotLightDirection5;
uniform float constant5;
uniform float linear5;
uniform float quadratic5;
uniform float intensity5;
uniform float spotLightCutOff5;


vec3 ambient;
float ambientStrength = 0.2f;
vec3 diffuse;
vec3 specular;
float specularStrength = 0.5f;
float shininess = 32.0f;

//day-night effect
float ambientIntensity = 1.0f;


void computeLightComponents()
{		
	vec3 cameraPosEye = vec3(0.0f);//in eye coordinates, the viewer is situated at the origin
	
	//transform normal
	vec3 normalEye = normalize(fNormal);	
	
	//compute light direction
	vec3 lightDirN = normalize(lightDir);
	
	//compute view direction 
	vec3 viewDirN = normalize(cameraPosEye - fPosEye.xyz);
		
	//compute ambient light
	ambient = ambientStrength * lightColor;
	
	//compute diffuse light
	diffuse = max(dot(normalEye, lightDirN), 0.0f) * lightColor;
	
	//compute specular light
	vec3 reflection = reflect(-lightDirN, normalEye);
	float specCoeff = pow(max(dot(viewDirN, reflection), 0.0f), shininess);
	specular = specularStrength * specCoeff * lightColor;

	//security lamp
	vec3 lightDirN1 = normalize(pointLightPosition1 - fragWorldPos.xyz);

	//cpmpute distance to light
	float distance = length(pointLightPosition1 - fragWorldPos.xyz);

	//compute attenuation
	float attenuation = 1.0f / (constant1 + linear1 * distance + quadratic1 * (distance * distance));

	//compute ambient light
	ambient += (attenuation * ambientStrength * pointLightColor1) * intensity1;

	//compute diffuse light
	diffuse += attenuation * max(dot(normalEye, lightDirN1), 0.0f) * pointLightColor1 * intensity1;

	//compute specular light
	specular += attenuation * specularStrength * specCoeff * pointLightColor1 * intensity1;

	//street lamp
	vec3 lightDirN2 = normalize(pointLightPosition2 - fragWorldPos.xyz);
	float distance2 = length(pointLightPosition2 - fragWorldPos.xyz);
	attenuation = 1.0f / (constant2 + linear2 * distance2 + quadratic2 * (distance2 * distance2));
	ambient += (attenuation * ambientStrength * pointLightColor2) * intensity2;
	diffuse += attenuation * max(dot(normalEye, lightDirN2), 0.0f) * pointLightColor2 * intensity2;
	specular += attenuation * specularStrength * specCoeff * pointLightColor2 * intensity2;

	//siren light
	vec3 lightDirN3 = normalize(pointLightPosition3 - fragWorldPos.xyz);
	float distance3 = length(pointLightPosition3 - fragWorldPos.xyz);
	attenuation = 1.0f / (constant3 + linear3 * distance3 + quadratic3 * (distance3 * distance3));
	ambient += (attenuation * ambientStrength * pointLightColor3) * intensity3;
	diffuse += attenuation * max(dot(normalEye, lightDirN3), 0.0f) * pointLightColor3 * intensity3;
	specular += attenuation * specularStrength * specCoeff * pointLightColor3 * intensity3;

	//cat light
	
	lightDirN1 = normalize(pointLightPosition4 - fragWorldPos.xyz);
	float spotFactor = dot(lightDirN1, normalize(-spotLightDirection));

	if(spotFactor > spotLightCutOff){

		float attenuationSpot = (1.0f - (1.0f - spotFactor) / (1.0f - spotLightCutOff));
		distance = length(pointLightPosition4 - fragWorldPos.xyz);
		attenuation = 1.0f / (constant4 + linear4 * distance + quadratic4 * (distance * distance));
		ambient += (attenuation * ambientStrength * pointLightColor4) * intensity4 * attenuationSpot;
		diffuse += attenuation * max(dot(normalEye, lightDirN1), 0.0f) * pointLightColor4 * intensity4 * attenuationSpot;
		specular += attenuation * specularStrength * specCoeff * pointLightColor4 * intensity4 * attenuationSpot;
		
	}

	//headlight
	vec3 lightDirN5 = normalize(pointLightPosition5 - fPosEye.xyz);
	float spotFactor5 = dot(lightDirN5, normalize(-spotLightDirection5));

	if(spotFactor5 > spotLightCutOff5){
		
		float attenuationSpot5 = (1.0f - (1.0f - spotFactor5) / (1.0f - spotLightCutOff5));
		float distance5 = length(pointLightPosition5 - fPosEye.xyz);
		attenuation = 1.0f / (constant5 + linear5 * distance5 + quadratic5 * (distance5 * distance5));
		ambient += (attenuation * ambientStrength * pointLightColor5) * intensity5 * attenuationSpot5;
		diffuse += attenuation * max(dot(normalEye, lightDirN5), 0.0f) * pointLightColor5 * intensity5 * attenuationSpot5;
		specular += attenuation * specularStrength * specCoeff * pointLightColor5 * intensity5 * attenuationSpot5;
	}

	//float distance5 = length(pointLightPosition5 - fPosEye.xyz);
	//attenuation = 1.0f / (constant5 + linear5 * distance5 + quadratic5 * (distance5 * distance5));
	//ambient += (attenuation * ambientStrength * pointLightColor5) * intensity5;
	//diffuse += attenuation * max(dot(normalEye, lightDirN5), 0.0f) * pointLightColor5 * intensity5;
	//specular += attenuation * specularStrength * specCoeff * pointLightColor5 * intensity5;
	


	
}

float computeShadow()
{
	vec3 normalizedCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
	normalizedCoords = normalizedCoords * 0.5f + 0.5f;
	
	float closestDepth = texture(shadowMap, normalizedCoords.xy).r;
	float currentDepth = normalizedCoords.z;
	float bias = 0.005f;
	float shadow = currentDepth - bias > closestDepth ? 1.0f : 0.0f;
	
	if(normalizedCoords.z > 1.0f)
		shadow = 0.0f;

	return shadow;
}

//fog
float computeFog(){

	float fogDensity = 0.05f;
	float fragmentDistance = length(fPosEye);
	float fogFactor = exp(-pow(fragmentDistance * fogDensity, 2));

	return clamp(fogFactor, 0.0f, 1.0f);
}

//create rain
 


void main() 
{
	computeLightComponents();
	
	vec3 baseColor = vec3(0.9f, 0.35f, 0.0f);//orange
	
	ambient *= texture(diffuseTexture, fTexCoords).rgb;
	diffuse *= texture(diffuseTexture, fTexCoords).rgb;
	specular *= texture(specularTexture, fTexCoords).rgb;



	float shadow = computeShadow();
	vec3 color = min((ambient + (1.0f - shadow) * diffuse) + (1.0f - shadow) * specular, 1.0f);
    
	if(fogEnabled){
		float fogFactor = computeFog();
		vec4 fogColor = vec4(0.5f, 0.5f, 0.5f, 1.0f);
		fColor = mix(vec4(fogColor.rgb,1.0f), vec4(color,1.0), fogFactor);
	}else{
		fColor = vec4(color, 1.0f);
	}
	
}
