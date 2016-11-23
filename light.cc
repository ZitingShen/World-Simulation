#include "light.h"

using namespace std;

void update_light(glm::vec3& sun_pos, LIGHT& THE_LIGHT){
	if (glm::sin(sun_pos[2])*SUN_RADIUS + SUN_SIZE < 0){
		THE_LIGHT.ifNight = 1;
	} else if (glm::sin(sun_pos[2])*SUN_RADIUS - 5*SUN_SIZE > 0) {
		THE_LIGHT.ifNight = 0;
	} else {
		THE_LIGHT.ifNight = 1 - (glm::sin(sun_pos[2])*SUN_RADIUS + SUN_SIZE)/(6*SUN_SIZE);
	}
}

void initialise_spot_light(spotlight& s_l, glm::vec4 pos, glm::vec3 direction){
  s_l.pos = glm::vec4(glm::vec3(pos), 1.0f);
  s_l.coneAngle = ANGLE;
  s_l.coneDirection = normalize(direction);
}

void update_spot_light(spotlight& s_l,
                       int width, int height,
                       glm::mat4 MV_MAT,
                       glm::mat4 PROJ_MAT,
                       mouse& mouse_pos,
                       glm::vec4 pos,
                       glm::vec3 direction,
                       bool fp){
  if (!fp){ // just follow the boid and its direction
    s_l.pos = glm::vec4(glm::vec3(pos), 1.0f);
    //s_l.pos = glm::vec4(0, 0, 8000.0f, 1);
    s_l.coneDirection = normalize(direction);
  }else{ // other wise the headlight follows our cursor

    //glm::vec4 viewport(0, 0, width, height);
    //glm::vec3 win(width, height, CAMERA_NEAR);
    //glm::vec3 cursor_world_coord = glm::unProject(win, MV_MAT, PROJ_MAT, viewport);

    float mouseX = mouse_pos.x_pos / (width  * 0.5f) - 1.0f;
    float mouseY = mouse_pos.y_pos / (height * 0.5f) - 1.0f;
    glm::mat4 inv = glm::inverse(PROJ_MAT * MV_MAT);
    glm::vec4 screenPos = glm::vec4(-mouseX, mouseY, 1.0f, 1.0f);
    glm::vec4 worldPos = inv * screenPos;

    s_l.coneDirection = glm::normalize(glm::vec3(worldPos));
    s_l.pos = glm::vec4(glm::vec3(pos), 1);
  }
}
