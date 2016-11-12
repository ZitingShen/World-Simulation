#ifndef VIEW_H
#define VIEW_H

#include "common.h"
#include "goal.h"
#include "boid.h"
#include "glm/gtc/matrix_transform.hpp"		       

typedef enum viewMode {DEFAULT, TRAILING, SIDE} viewMode;

void change_view(glm::mat4& PROJ_MAT, viewMode viewmode, 
	vector<BOID>& flock, GOAL& goal, int index);
#endif
