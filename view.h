#ifndef VIEW_H
#define VIEW_H

#include "common.h"
#include "boid.h"
#include "goal.h"
#include "glm/gtc/matrix_transform.hpp"

enum viewMode {DEFAULT, TRAILING, SIDE, FP};

void change_view(glm::mat4& PROJ_MAT,
                 viewMode vm,
	               std::vector<BOID>& flock,
                 GOAL& goal,
                 glm::vec3& fp_direction);
#endif
