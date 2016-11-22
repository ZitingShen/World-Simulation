#ifndef VIEW_H
#define VIEW_H

#include "common.h"
#include "boid.h"
#include "goal.h"
#include "glm/gtc/matrix_transform.hpp"

enum viewMode {DEFAULT, TRAILING, SIDE, FP, TRAIL_FP};

void change_view(glm::mat4& PROJ_MAT,
                 viewMode vm,
                 float tower,
	               std::vector<BOID>& flock,
                 GOAL& goal,
                 glm::vec3& island_centre,
                 glm::vec3& eye);

void zoom_in(float& tower);
void zoom_out(float& tower);
void zoom_reset(float& tower);
#endif
