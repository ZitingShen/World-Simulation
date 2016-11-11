#ifndef GOAL_H
#define GOAL_H

#include "common.h"

const GLfloat CUBE_VERTICES[][3] = {
                           {-GOAL_SIZE, -GOAL_SIZE, GOAL_SIZE},  
                           {-GOAL_SIZE, GOAL_SIZE, GOAL_SIZE}, 
                           {GOAL_SIZE, GOAL_SIZE, GOAL_SIZE}, 
                           {GOAL_SIZE, -GOAL_SIZE, GOAL_SIZE}, 
                           {-GOAL_SIZE, -GOAL_SIZE, -GOAL_SIZE},
                           {-GOAL_SIZE, GOAL_SIZE, -GOAL_SIZE}, 
                           {GOAL_SIZE, GOAL_SIZE, -GOAL_SIZE}, 
                           {GOAL_SIZE, -GOAL_SIZE, -GOAL_SIZE}};

const GLubyte CUBE_INDICES[36] = {0, 3, 2,
                                0, 2, 1,
                                2, 3, 7,
                                2, 7, 6,
                                0, 4, 7,
                                0, 7, 3,
                                1, 2, 6,
                                1, 6, 5,
                                4, 5, 6,
                                4, 6, 7,
                                0, 1, 5,
                                0, 5, 4};

const glm::vec3 DEFAULT_GOAL_SPAWN_VELOCITY = glm::vec3(10.0f, 10.0f, 0.01f); // initiated with a positive speed on Z-axis
const glm::vec3 DEFAULT_GOAL_SPAWN_POSITION = glm::vec3 (0.0f, 5000.0f, 3000.0f);

typedef struct _goal{
  glm::vec3 pos;
  glm::vec3 velocity;
  bool MOVE_ALONG_X_POSITIVE;  // to control goal
  bool MOVE_ALONG_X_NEGATIVE;
  bool MOVE_ALONG_Y_POSITIVE;
  bool MOVE_ALONG_Y_NEGATIVE;
  bool ACCELERATE;
  bool DECELERATE;

  _goal();
  void update_goal_velocity();
  void update_goal_pos();
  void teleport_goal();
  void print_goal();
} GOAL;
#endif
