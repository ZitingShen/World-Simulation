#ifndef SUN_H 
#define SUN_H
#include "common.h"
#include "fin.h"
#include "read_ppm.h"
#include <time.h>

void init_sun_mesh(MESH& mesh, GLuint shader, glm::mat4& PROJ_MAT);
void draw_a_sun(glm::vec3& sun_pos, MESH& mesh, GLuint shader, glm::mat4& MV_MAT, LIGHT THE_LIGHT);
void update_sun_pos(glm::vec3& sun_pos);
#endif