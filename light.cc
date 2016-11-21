#include "light.h"

using namespace std;

void update_light(glm::vec3& sun_pos, LIGHT& THE_LIGHT){
	if (glm::sin(sun_pos[2])*SUN_RADIUS + SUN_SIZE < 0){
		THE_LIGHT.ifNight = 1;
	} else {
		THE_LIGHT.ifNight = 0;
	}
}

void initialise_spot_light(spotlight& s_l, glm::vec4 pos, glm::vec3 direction){
  s_l.pos = glm::vec4(glm::vec3(pos) + normalize(direction) * BOID_SIZE, 1);
  s_l.coneAngle = ANGLE;
  s_l.coneDirection = normalize(direction);
}

void update_spot_light(spotlight& s_l,
                       double xpos, double ypos,
                       mouse& mouse_pos,
                       glm::vec4 pos,
                       glm::vec3 direction,
                       bool vm){
  float theta = 0;
  float radius = static_cast<float>(BOID_SIZE);
  glm::vec3 rotation;
  if (!vm){ // just follow the boid and its direction
    s_l.pos = glm::vec4(glm::vec3(pos) + normalize(direction) * BOID_SIZE, 1);
    s_l.coneDirection = normalize(direction);
  }else{
    theta = sqrt(pow(xpos - mouse_pos.x_pos, 2) + pow(ypos - mouse_pos.y_pos, 2)) / radius;
    rotation = glm::rotate(s_l.coneDirection, theta, direction);

    s_l.pos =glm::vec4(glm::vec3(pos) + normalize(direction) * BOID_SIZE, 1);
    s_l.coneDirection = normalize(rotation);
  }
  mouse_pos.x_pos = xpos;
  mouse_pos.y_pos = ypos;
}
