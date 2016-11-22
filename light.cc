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
  s_l.pos = glm::vec4(glm::vec3(pos) + normalize(direction) * 3.0f * BOID_SIZE, 1);
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
                       bool vm){
  if (!vm){ // just follow the boid and its direction
    s_l.pos = glm::vec4(glm::vec3(pos) + normalize(direction) * 3.0f * BOID_SIZE, 1);
    s_l.coneDirection = normalize(direction);
  }else{
    //theta = sqrt(pow(xpos - mouse_pos.x_pos, 2) + pow(ypos - mouse_pos.y_pos, 2)) / radius;
    //rotation = glm::rotate(s_l.coneDirection, theta, direction);
    //rotation = glm::rotate(direction, theta, s_l.coneDirection);
    //cout << mouse_pos.x_pos << " " << mouse_pos.y_pos << endl;
    //glm::vec4 viewport = glm::vec4(0.0f, 0.0f, width, height);

    float mouseX = mouse_pos.x_pos / (width  * 0.5f) - 1.0f;
    float mouseY = mouse_pos.y_pos / (height * 0.5f) - 1.0f;
    //cout << mouseX << " " << mouseY << endl;
    glm::mat4 inv = glm::inverse(PROJ_MAT * MV_MAT);
    glm::vec4 screenPos = glm::vec4(-mouseX, mouseY, 1.0f, 1.0f);
    glm::vec4 worldPos = inv * screenPos;

    s_l.coneDirection = glm::normalize(glm::vec3(worldPos));
    s_l.pos = glm::vec4(glm::vec3(pos) + normalize(direction) * 3.0f *  BOID_SIZE, 1);
  }

  //std::cout << "Spot light pos " << s_l.pos.x << " "<<  s_l.pos.y << " " <<  s_l.pos.z << " " << endl;
  //std::cout << "Spot direction" << s_l.coneDirection.x << " "<<  s_l.coneDirection.y << " " <<  s_l.coneDirection.z << " " << endl;
}
