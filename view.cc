#include "view.h"

void change_view(glm::mat4& MV_MAT, viewMode viewmode, 
  vector<BOID>& flock, GOAL& goal, int index) {
  glm::vec3 centroid = (flock_centroid(flock, 0) 
    + flock_centroid(flock, 1))*0.5f;
  glm::vec3 midpoint = (mid_point(flock, goal, 0) 
    + mid_point(flock, goal, 1))*0.5f;
  float distance = (get_d(flock, goal, 0) 
    + get_d(flock, goal, 1))*0.5f;
  glm::vec3 flock_direction = glm::normalize((get_u(flock, goal, 0) 
    + get_u(flock, goal, 1))*0.5f);
  glm::vec3 velocity_direction = glm::normalize(
    get_average_v(flock, index)); 

  glm::vec3 eye = glm::vec3(0, 2500, TOWER_HEIGHT);
  glm::vec3 center = glm::vec3(midpoint[0], midpoint[1], midpoint[2]);
  glm::vec3 up = glm::vec3(0, 0, 1);

  switch(viewmode) {
    case TRAILING:
    centroid = (flock_centroid(flock, index));
    distance = (get_d(flock, goal, index));
    distance = (distance < 4000)?4000:distance;
    //std::cout << distance << std::endl;
    eye = centroid
          - flock_direction*distance*0.9f
          - velocity_direction*distance*1.1f
          + up*TOWER_HEIGHT*0.8f;
    break;

    case SIDE:{
    glm::vec3 side_v = glm::cross(flock_direction, up);
    eye = centroid
          + glm::normalize(side_v)*distance*3.0f
          + up*distance*0.3f;
    }
    break;
    default:
    break;
  }
  MV_MAT = glm::lookAt(eye, center, up);
}
