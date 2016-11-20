#include "light.h"

using namespace std;

void update_light(glm::vec3& sun_pos, LIGHT& THE_LIGHT) {
	if (glm::sin(sun_pos[2])*SUN_RADIUS + SUN_SIZE < 0) {
		THE_LIGHT.ifNight = 1;
	} else {
		THE_LIGHT.ifNight = 0;
	}
}

void initialise_spot_light(spot_light& s_l, vector<BOID>& flock){
  s_l.pos = flock[0].pos + normalize(flock[0].velocity) * BOID_SIZE;
  s_l.colour = glm::vec3(1.0, 1.0, 1.0);
  s_l.intensity = 1.0;
  s_l.coneAngle = 0.8;
  s_l.coneDirection = normalize(flock[0].velocity);
}

void update_spot_light(spot_light& s_l,
                       mouse mouse_pos,
                       vector<BOID>& flock,
                       viewMode vm){
  //double theta; // in radians
  //glm::vec3 rotate_normal;
  //if (viewMode != FP){
    s_l.pos = flock[0].pos + normalize(flock[0].velocity) * BOID_SIZE;
    s_l.coneDirection = normalize(flock[0].velocity);
  //}else{
  //  s_l.pos = flock[0].pos + normalize(flock[0].velocity) * BOID_SIZE;
  //  s_l.coneDirection = normalize(flock[0].velocity);
  //  //theta = (2 * sqrt( (xpos - mouse_pos.x_pos) * (xpos - mouse_pos.x_pos)
    //             +(ypos - mouse_pos.y_pos) * (ypos - mouse_pos.y_pos))) / static_cast<double>(BOID_SIZE);
  //}
}
