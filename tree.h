#ifndef TREE_H 
#define TREE_H
#include "common.h"
#include "island.h"
#include "fin.h"
#include "read_ppm.h"
#include <time.h>

void init_tree_mesh(MESH& mesh, MESH& island_mesh, const char *texture, GLuint shader, 
  glm::mat4& PROJ_MAT, vector<glm::vec3>& tree_pos, int TREE_NUM, int &TEXTURE_COUNTER);
void draw_tree(MESH& mesh, GLuint shader, glm::mat4& MV_MAT, LIGHT THE_LIGHT,
               spotlight SPOT_LIGHT, vector<glm::vec3>& tree_pos, float TREE_SIZE);
#endif