#ifndef VIEW_H
#define VIEW_H

#include "common.h"
#include "goal.h"
#include "boid.h"
#include "glm/gtc/matrix_transform.hpp"

enum viewMode {DEFAULT, TRAILING, SIDE, FP};

void change_view(glm::mat4& PROJ_MAT, viewMode vm,
	                vector<BOID>& flock, GOAL& goal);
#endif
