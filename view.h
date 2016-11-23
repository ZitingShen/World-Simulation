#ifndef VIEW_H
#define VIEW_H

#include "common.h"
#include "boid.h"
#include "goal.h"
#include "glm/gtc/matrix_transform.hpp"

enum viewMode {DEFAULT, TRAILING, SIDE, FP, ENV};

const float CAMERA_SPEED_CAP = 200.0f;

class camera{
public:
  glm::vec3 eye;
  glm::vec3 center;
  glm::vec3 up;
  glm::vec3 env_cam_pos;
  viewMode current_vm;
  float tower;

  camera(float tower);

  void change_view(glm::mat4& PROJ_MAT,
	                 std::vector<BOID>& flock,
                   GOAL& goal,
                   glm::vec3& island_centre,
                   glm::vec3& env_centre,
                   glm::vec3& cone_direction);

  void zoom_in();
  void zoom_out();
  void zoom_reset();
};

#endif
