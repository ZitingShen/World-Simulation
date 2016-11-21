#version 150

out vec4 fColor;
in vec3 pos_eye, normal_eye;
in vec2 texCoord;

uniform vec4 sp_position; //sp position
uniform vec3 coneDirection;

uniform float coneAngle;

uniform sampler2D tex;
uniform vec4 LightPosition;
uniform float Shineness;

vec3 apply_spot_light(vec4 position, vec3 direction, float coneAngle){
  vec3 surface_to_sl = normalize(position.xyz);
  float angle = degrees(acos(dot(-surface_to_sl, normalize(direction))));
  if(angle > coneAngle){
    return vec3(0.0, 0.0, 0.0); //out of cone
  }

  vec3 RedLight = normalize(vec3(1.0, 1.0, 1.0));

	vec3 L = normalize(position.xyz - pos_eye);
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

	vec3 L = normalize(LightPosition.xyz - pos_eye);
	vec3 E = normalize(-pos_eye);
	vec3 H = normalize(L + E);
	vec3 Ia = whiteLight;
	vec3 Id = whiteLight*max(dot(L, normal_eye), 0.0);
	vec3 Is = whiteLight*pow(max(dot(normal_eye, H), 0.0), Shineness);

	//discard the specular highlight if the
	//vertex is not facing light
	if(dot(L, normal_eye) < 0.0 ) Is = vec3(0.0, 0.0, 0.0);

  // calculating spotlight
  vec3 spotlighting = apply_spot_light(sp_position, coneDirection, coneAngle);

  vec4 shadeLight = vec4(Ia+Id+Is + spotlighting, 1.0);
	vec4 shadeTex = vec4(texture(tex, texCoord).rgb, 1.0);
	fColor = shadeLight*shadeTex;
}
