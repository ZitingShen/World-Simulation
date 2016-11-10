#ifndef GOAL_H
#define GOAL_H

#include "common.h"

const glm::vec3 DEFAULT_GOAL_SPAWN_VELOCITY(10, 10, 0.01); // initiated with a positive speed on Z-axis
const glm::vec3 DEFAULT_GOAL_SPAWN_POSITION(0, 5000, 3000);

typedef struct _goal{
  glm::vec3 pos;
  glm::vec3 velocity;
  bool MOVE_ALONG_X_POSITIVE;  // to control goal
  bool MOVE_ALONG_X_NEGATIVE;
  bool MOVE_ALONG_Y_POSITIVE;
  bool MOVE_ALONG_Y_NEGATIVE;
  bool ACCELERATE;
  bool DECELERATE;

  _goal()
} GOAL;

void update_goal_velocity(GOAL& a_goal);
void teleport_goal(GOAL& a_goal);
void update_goal_pos(GOAL& a_goal);
void print_goal(GOAL& a_goal);
#endif
