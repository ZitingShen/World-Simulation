#version 150

in vec3 vPosition;
in vec3 vNormal;
in vec2 vTex;

out vec3 pos_eye, normal_eye;
out vec2 texCoord;
out vec3 R;

uniform mat4 ModelView;
uniform mat4 Projection;
uniform vec3 Camera;
void main(){
	pos_eye = vec3(ModelView*vec4(vPosition, 1.0));
	normal_eye = normalize(vec3(ModelView*vec4(vNormal, 0.0)));
	texCoord = vTex;
	gl_Position = Projection*vec4(pos_eye, 1.0);
	vec3 R = reflect(Camera-vPosition, vNormal);
}