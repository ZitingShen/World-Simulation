#version 150

out vec4 fColor;
in vec3 pos_eye, normal_eye;
in vec2 texCoord;
in vec3 vPosition;

uniform sampler2D tex0, tex1, tex2, tex3, tex4, tex5;
uniform vec4 LightPosition;
uniform float Shineness;
uniform int ifNight;
uniform int ifSnow;

void main() {
	vec3 whiteLight = normalize(vec3(1.0, 1.0, 1.0));
	vec3 blueLight = normalize(vec3(0.1, 0.1, 0.2));

	vec3 L = normalize(LightPosition.xyz - pos_eye);
	vec3 E = normalize(-pos_eye);
	vec3 H = normalize(L + E);
	vec3 Ia, Id, Is;
	if (ifNight == 1) {
		Ia = blueLight;
		Id = vec3(0, 0, 0);
		Is = vec3(0, 0, 0);
	} else {
		Ia = whiteLight;
		Id = whiteLight*max(dot(L, normal_eye), 0.0);
		Is = whiteLight*pow(max(dot(normal_eye, H), 0.0), Shineness);
	}

	//discard the specular highlight if the
	//vertex is not facing light
	if(dot(L, normal_eye) < 0.0 ) Is = vec3(0.0, 0.0, 0.0);

	vec4 shadeLight = vec4(Ia+Id+Is, 1.0);
	vec4 shadeTex = vec4(texture(tex0, texCoord).rgb, 1.0);
	if (ifSnow == 1 && vPosition[2] > 20.0)
		shadeTex = vec4(texture(tex1, texCoord).rgb, 1.0);
	fColor = shadeLight*shadeTex;
}