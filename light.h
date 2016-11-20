#ifndef LIGHT_H
#define LIGHT_H

#include "light.h"
#include "boid.h"
#include "view.h"
#include "common.h"
#include "math.h"
#include <vector>

typedef struct _light{
	glm::vec4 light0 = glm::vec4(10.0f, 10.0f, 100.0f, 0);
 	GLint ifNight = 0;
} LIGHT;

struct spotlight{
  glm::vec3 pos;
  glm::vec3 colour;
  float intensity;  // for dropping of & out of coneAngle
  float coneAngle;
  glm::vec3 coneDirection;
};

struct mouse{
  double x_pos;
  double y_pos;
}

void update_light(glm::vec3& sun_pos, LIGHT& THE_LIGHT);

void initialise_spot_light(spot_light& s_l, vector<BOID>& flock);

void update_spot_light(spot_light& s_l,
                       mouse mouse_pos,
                       vector<BOID>& flock,
                       viewMode& vm);

#endif
