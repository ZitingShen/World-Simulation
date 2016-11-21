#ifndef ANTI_COLLISION_H
#define ANTI_COLLISION_H

#include "common.h"
#include "boid.h"
#include "fin.h"

const float RANGE = 3000;
const float DETERRENCE_WEIGHT = 0.1;

struct PREDATOR{
  glm::vec3 pos;
  //glm::vec4 velocity;
  float deterrence_range;
};

void create_predators(vector<PREDATOR>& predators, vector<MESH>& obstacles);
void apply_predator_deterrence(vector<PREDATOR>& predators, vector<BOID> a_flock);

//void move_predator(List* a_flock, PREDATOR* a_predator, GOAL* a_goal, bool& guardian);
//void draw_predator(PREDATOR* a_predator, bool& guardian, GLfloat mv_mat[]);
//void delete_predator(PREDATOR* a_predator, bool& guardian);

#endif
