#ifndef GOAL_H
#define GOAL_H

#include "common.h"

const vec4 DEFAULT_GOAL_SPAWN_VELOCITY(10, 10, 0.01, 0); // initiated with a positive speed on Z-axis
const vec4 DEFAULT_GOAL_SPAWN_POSITION(0, 5000, 3000, 1);

typedef struct _goal{
  vec4 pos;
  vec4 velocity;
  bool MOVE_ALONG_X_POSITIVE;  // to control goal
  bool MOVE_ALONG_X_NEGATIVE;
  bool MOVE_ALONG_Y_POSITIVE;
  bool MOVE_ALONG_Y_NEGATIVE;
  bool ACCELERATE;
  bool DECELERATE;
} GOAL;


GOAL* new_goal();
void update_goal_velocity(GOAL* a_goal);
void teleport_goal(GOAL* a_goal);
void update_goal_pos(GOAL* a_goal);
void print_goal(GOAL* a_goal);
#endif
