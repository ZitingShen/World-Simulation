#ifndef OCEAN_H
#define OCEAN_H
#include "common.h"
#include "fin.h"
#include <time.h>
#include "read_ppm.h"

const GLfloat OCEAN_VERTICES[][3] = {
                           {-OCEAN_SIZE, -OCEAN_SIZE, 0},  
                           {-OCEAN_SIZE, OCEAN_SIZE, 0}, 
                           {OCEAN_SIZE, OCEAN_SIZE, 0}, 
                           {OCEAN_SIZE, -OCEAN_SIZE, 0}};

const GLfloat OCEAN_TEX[][2] = {{0.0f, 0.0f}, 
                                 {0.0f, 1.0f}, 
                                 {1.0f, 1.0f}, 
                                 {1.0f, 0.0f}};

const GLubyte OCEAN_INDICES[6] = {0, 3, 2,
                                0, 2, 1};

void init_ocean_mesh(MESH& mesh, GLuint shader, glm::mat4& PROJ_MAT);
void draw_ocean(MESH& mesh, GLuint shader, glm::mat4& MV_MAT, LIGHT THE_LIGHT);
#endif