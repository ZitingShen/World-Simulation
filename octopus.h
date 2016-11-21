#ifndef OCTOPUS_H
#define OCTOPUS_H
#include "common.h"
#include "fin.h"
#include "read_ppm.h"

const glm::vec3 OCTOPUS_POS = glm::vec3(1000.0f, 1000.0f, 1000.0f);

void init_octopus_mesh(MESH& mesh, GLuint shader, glm::mat4& PROJ_MAT);
void draw_octopus(MESH& mesh, GLuint shader, glm::mat4& MV_MAT, LIGHT THE_LIGHT, 
	glm::vec3& camera);
#endif