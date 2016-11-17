#version 150

out vec4 fColor;
in vec3 pos_eye, normal_eye;
in vec2 texCoord;

uniform sampler2D tex;
uniform vec4 LightPosition;
uniform float Shineness;

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

	vec4 shadeLight = vec4(Ia+Id+Is, 1.0);
	vec4 shadeTex = vec4(texture(tex, texCoord).rgb, 1.0);
	fColor = shadeLight*shadeTex;
}