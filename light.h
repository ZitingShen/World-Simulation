#ifndef LIGHT_H
#define LIGHT_H

#include "common.h"
#include "math.h"
#include "glm/glm.hpp"
#include "glm/gtx/rotate_vector.hpp" // for rotation

const float STEER_ANGLE = 0.1; // in radian
const float ANGLE = 0.6;

struct LIGHT{
	glm::vec4 light0 = glm::vec4(10.0f, 10.0f, 100.0f, 0);
 	GLint ifNight = 0;
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
                       double xpos, double ypos,
                       mouse& mouse_pos,
                       glm::vec4 pos,
                       glm::vec3 direction,
                       bool vm);

#endif
