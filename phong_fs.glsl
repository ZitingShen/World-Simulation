#version 150

out vec4 fColor;
in vec3 fN;
in vec3 fL;
in vec3 fE;

uniform vec4 AmbientProduct, DiffuseProduct, SpecularProduct;
uniform mat4 ModelView;
uniform vec4 LightPosition;
uniform float Shininess;

void main() {
	// Normalize the input lighting vectors
	vec3 N = normalize(fN);
	vec3 E = normalize(fE);
	vec3 L = normalize(fL);
	vec3 H = normalize( L + E );
	vec4 ambient = AmbientProduct;
	float Kd = max(dot(L, N), 0.0);
	vec4 diffuse = Kd*DiffuseProduct;
	float Ks = pow(max(dot(N, H), 0.0), Shininess);
	vec4 specular = Ks*SpecularProduct;
	//discard the specular highlight if the
	//vertex is not facing light
	if( dot(L, N) < 0.0 )
		specular = vec4(0.0, 0.0, 0.0, 1.0);

	fColor = ambient + diffuse + specular;
	//fColor = ambient;
	fColor.a = 1.0;
}