#version 150

in vec3 vPosition;
in vec3 vNormal;

out vec3 pos_eye, normal_eye;
out vec2 texCoord;
out vec3 incident, worldNormal;

uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;
uniform vec3 Camera;
void main(){
	pos_eye = vec3(Model*View*vec4(vPosition, 1.0));
	normal_eye = normalize(vec3(Model*View*vec4(vNormal, 0.0)));
	gl_Position = Projection*vec4(pos_eye, 1.0);
	incident = normalize(Camera-vec3(Model*vec4(vPosition, 1.0f)));
	worldNormal = normalize(mat3(Model)*vNormal);
}