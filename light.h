#ifndef LIGHT_H
#define LIGHT_H

#include "common.h"
#include <iostream>

typedef struct _light{
	glm::vec4 light0 = glm::vec4(10.0f, 10.0f, 100.0f, 0);
 	GLfloat ifNight = 0;
} LIGHT;

void update_light(glm::vec3& sun_pos, LIGHT& THE_LIGHT);

#endif