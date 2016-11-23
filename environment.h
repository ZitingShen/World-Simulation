#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H
#include "common.h"
#include "fin.h"
#include "read_ppm.h"

void init_environment_mesh(MESH& mesh, GLuint shader, glm::mat4& PROJ_MAT, int &TEXTURE_COUNTER);
void draw_environment(MESH& mesh, GLuint shader, glm::mat4& MV_MAT, LIGHT THE_LIGHT, 
	glm::vec3& camera, spotlight SPOT_LIGHT, glm::vec3& environment_pos);
#endif
