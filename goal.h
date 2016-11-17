#ifndef GOAL_H
#define GOAL_H

#include "common.h"
#include "fin.h"

const GLfloat CUBE_VERTICES[][3] = {
                           {-GOAL_SIZE, -GOAL_SIZE, GOAL_SIZE},  
                           {-GOAL_SIZE, GOAL_SIZE, GOAL_SIZE}, 
                           {GOAL_SIZE, GOAL_SIZE, GOAL_SIZE}, 
                           {GOAL_SIZE, -GOAL_SIZE, GOAL_SIZE}, 
                           {-GOAL_SIZE, -GOAL_SIZE, -GOAL_SIZE},
                           {-GOAL_SIZE, GOAL_SIZE, -GOAL_SIZE}, 
                           {GOAL_SIZE, GOAL_SIZE, -GOAL_SIZE}, 
                           {GOAL_SIZE, -GOAL_SIZE, -GOAL_SIZE}};

const GLfloat A_GOAL_TEX[][2] = {{0.0f, 0.0f}, 
                                 {0.1f, 1.0f}, 
                                 {0.2f, 0.5f}, 
                                 {0.3f, 0.33f}, 
                                 {0.4f, 0.25f}, 
                                 {0.5f, 0.2f}, 
                                 {0.6f, 0.15f}, 
                                 {1.0f, 0.1f}};

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

const glm::vec3 DEFAULT_GOAL_SPAWN_VELOCITY = glm::vec3(0.0f, 0.0f, 0.01f);
const glm::vec3 DEFAULT_GOAL_SPAWN_POSITION = glm::vec3 (0.0f, 0.0f, 4000.0f);

class GOAL{
  public:
    glm::vec3 pos;
    glm::vec3 velocity;
    bool MOVE_ALONG_X_POSITIVE;  // to control goal
    bool MOVE_ALONG_X_NEGATIVE;
    bool MOVE_ALONG_Y_POSITIVE;
    bool MOVE_ALONG_Y_NEGATIVE;
    bool ACCELERATE;
    bool DECELERATE;

    GOAL();
};

void update_goal_velocity(GOAL& goal);
void update_goal_pos(GOAL& goal);
void teleport_goal(GOAL& goal);
void print_goal(GOAL& goal);
void init_goal_mesh(MESH& mesh, GLuint shader, glm::mat4& PROJ_MAT);
void draw_a_goal(GOAL& goal, MESH& mesh, GLuint shader, glm::mat4& MV_MAT, LIGHT THE_LIGHT);
#endif
