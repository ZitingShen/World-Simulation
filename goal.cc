#include "goal.h"

using namespace std;

GOAL::GOAL _goal(){
  pos = DEFAULT_GOAL_SPAWN_POSITION;
  velocity = DEFAULT_GOAL_SPAWN_VELOCITY;
  MOVE_ALONG_X_NEGATIVE = false;
  MOVE_ALONG_X_POSITIVE = false;
  MOVE_ALONG_Y_NEGATIVE = false;
  MOVE_ALONG_Y_POSITIVE = false;
  ACCELERATE = false;
  DECELERATE = false;
}

void GOAL::GOAL update_goal_velocity(){
  //float temp_z;
  if (MOVE_ALONG_X_POSITIVE){
    if (velocity[0] < 0){
      velocity[0] = -velocity[0];
    }
    velocity[0] += DEFAULT_ACCELERATION_MARGIN;
  }

  if (MOVE_ALONG_X_NEGATIVE){
    if (velocity[0] > 0){
      velocity[0] = -velocity[0];
    }
    velocity[0] -= DEFAULT_ACCELERATION_MARGIN;
  }

  if (MOVE_ALONG_Y_POSITIVE){
    if (velocity[1] < 0){
      velocity[1] = -velocity[1];
    }
    velocity[1] += DEFAULT_ACCELERATION_MARGIN;
  }

  if (MOVE_ALONG_Y_NEGATIVE){
    if (velocity[1] > 0){
      velocity[1] = -velocity[1];
    }
    velocity[1] -= DEFAULT_ACCELERATION_MARGIN;
  }

  if (ACCELERATE){
    velocity = velocity*float(1 + DEFAULT_ACCELERATION_FACTOR);
    velocity[2] = 0.01f; // do not accerlate on Z
  }

  if (DECELERATE){
    velocity =  velocity*float(1 - DEFAULT_ACCELERATION_FACTOR);
    velocity[2] = 0.01f; // do not accerlate on Z
  }
}

void GOAL::GOAL update_goal_pos(){
  pos += velocity;
}

void GOAL::GOAL teleport_goal(){
  pos[0] += rand() % 5000 - 10000;
  pos[1] += rand() % 5000 - 10000;
}

void GOAL::GOAL print_goal() {
  cout << "The goal's position: " << pos[0] << ", " << pos[1] << ", " 
  << pos[2] << endl;
  cout << "The goal's velocity: " << velocity[0] << ", " << velocity[1] << ", " 
  << velocity[2] << endl;
  cout << endl;
}

void draw_a_goal(){
  glUseProgram(boid_program);
  glBindVertexArray(vao_goal);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo_goal_indices);
  GLfloat trans[16];
  memcpy(trans, MV_MAT, sizeof(GLfloat)*16);
  myTranslate(trans, A_GOAL->pos[0], A_GOAL->pos[1], A_GOAL->pos[2]);
  glUniformMatrix4fv(proj, 1, GL_FALSE, PROJ_MAT);
  glUniformMatrix4fv(mv, 1, GL_FALSE, trans);
  glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_BYTE, (void*) 0);
}