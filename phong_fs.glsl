#version 150

out vec4 fColor;
in vec3 pos_eye, normal_eye;
in vec2 texCoord;
in vec3 vPosition;

uniform vec4 sp_position; //sp position
uniform vec3 coneDirection;
uniform float coneAngle;

uniform sampler2D tex0, tex1, tex2, tex3, tex4, tex5;

uniform vec4 LightPosition;
uniform float Shineness;
uniform float ifNight;
uniform int ifSnow;

vec3 apply_spot_light(vec4 light_position, vec3 direction, float coneAngle){
  vec3 surface_to_sl = light_position.xyz - vPosition;
  float angle = degrees(acos(dot(-surface_to_sl, normalize(direction))));
  if(angle > coneAngle){
    return vec3(0.0, 0.0, 0.0); //out of cone
  }

  vec3 RedLight = normalize(vec3(1.0, 0.0, 0.0));

	vec3 L = normalize(light_position.xyz - pos_eye);
	vec3 E = normalize(-pos_eye);
	vec3 H = normalize(L + E);
	vec3 Ia = RedLight;
	vec3 Id = RedLight*max(dot(L, normal_eye), 0.0);
	vec3 Is = RedLight*pow(max(dot(normal_eye, H), 0.0), Shineness);

	//discard the specular highlight if the
	//vertex is not facing light
	if(dot(L, normal_eye) < 0.0 ) Is = vec3(0.0, 0.0, 0.0);

  return Ia + Id + Is;
}

void main() {
	vec3 whiteLight = normalize(vec3(1.0, 1.0, 1.0));
	vec3 blueLight = normalize(vec3(0.1, 0.1, 0.2));

	vec3 L = normalize(LightPosition.xyz - pos_eye);
	vec3 E = normalize(-pos_eye);
	vec3 H = normalize(L + E);
	vec3 Ia = ifNight*blueLight + (1-ifNight)*whiteLight;
	vec3 Id = (1-ifNight)*whiteLight*max(dot(L, normal_eye), 0.0);
	vec3 Is = (1-ifNight)*whiteLight*pow(max(dot(normal_eye, H), 0.0), Shineness);

	//discard the specular highlight if the
	//vertex is not facing light
	if(dot(L, normal_eye) < 0.0 ) Is = vec3(0.0, 0.0, 0.0);

  // calculating spotlight
  vec3 spotlighting = apply_spot_light(sp_position, coneDirection, coneAngle);

  vec4 shadeLight = vec4(Ia+Id+Is + spotlighting, 1.0);
	vec4 shadeTex = vec4(texture(tex0, texCoord).rgb, 1.0);
	//if (ifSnow == 1)
	//	shadeTex = vec4(texture(tex1, texCoord).rgb, 1.0);
	fColor = shadeLight*shadeTex;

}
