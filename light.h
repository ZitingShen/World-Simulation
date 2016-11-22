#ifndef LIGHT_H
#define LIGHT_H

#include "common.h"
#include "math.h"
#include "glm/glm.hpp"
#include "glm/gtx/rotate_vector.hpp" // for rotation
#include <iostream>

const float STEER_ANGLE = 0.1; // in radian
const float ANGLE = 0.1 * M_PI; // 18 * 2 = 36 degrees

struct LIGHT{
	glm::vec4 light0 = glm::vec4(10.0f, 10.0f, 100.0f, 0);
 	GLfloat ifNight = 0;
};

struct spotlight{
  glm::vec4 pos;
  float coneAngle;
  glm::vec3 coneDirection;
};

struct mouse{
  double x_pos;
  double y_pos;
};

void update_light(glm::vec3& sun_pos, LIGHT& THE_LIGHT);

void initialise_spot_light(spotlight& s_l, glm::vec4 pos, glm::vec3 direction);

void update_spot_light(spotlight& s_l,
                       int width, int height,
                       glm::mat4 MV_MAT,
                       glm::mat4 PROJ_MAT,
                       mouse& mouse_pos,
                       glm::vec4 pos,
                       glm::vec3 direction,
                       bool vm);

#endif
