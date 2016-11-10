#ifndef BOID_H 
#define BOID_H

#include "list.h"
#include "goal.h"
#include "common.h"
#include <time.h>
#include <limits>
#include <algorithm>

const glm::vec3 SPAWN_POSITION_I = new glm::vec3(1000.0f, 1000.0f, 3000.0f);
const glm::vec3 SPAWN_POSITION_II = new glm::vec3(-1000.0f, -1000.0f, 3000.0f);
 //initial speed parallel with y-axis
const glm::vec3 SPAWN_VELOCITY = new glm::vec3(0.0f, 0.01f, 0.0f);

const glm::vec3 centroid_init = new glm::vec3(0.0f, 0.0f, 0.0f);
const glm::vec3 head_init = new glm::vec3(0.0f, BOID_SIZE*2, 0.0f);
const glm::vec3 left_init = new glm::vec3(-BOID_SIZE, -BOID_SIZE, 0.0f);
const glm::vec3 right_init = new glm::vec3(BOID_SIZE, -BOID_SIZE, 0.0f);

/* Dynamically dealing with Weights */
const float SCATTERING = 0.8f * PARTNER_RADIUS; // too far away from partner
const float COLLIDING  = 0.2f * PARTNER_RADIUS; // too close to partner
const float FLOCK_RAIUS_CAP = 10.0f * BOID_SIZE;
const float APPROACHING_GOAL = 20.0f * GOAL_SIZE;
const float Z_SPEED_CAP = 20.0f;
const float BOID_SPEED_FLOOR = 55.0f;
const float PREDATOR_SPEED_CAP = 300.0f;

typedef struct _boid{
  int flock_index;  
  glm::vec3 pos;
  glm::vec3 velocity;   
  float partner_radius;// the radius within which it looks for partners

  _boid();
} BOID;

bool is_partner(BOID* source, BOID* target);
void update_velocity(vector<BOID>& a_flock, GOAL& a_goal);
void update_pos(vector<BOID>& a_flock);

glm::vec3 get_current_pos(BOID* a_boid);
glm::vec3 flock_centroid(vector<BOID>& a_flock, int flock_index);
glm::vec3 mid_point(vector<BOID>& a_flock, GOAL& a_goal, int flock_index);
glm::vec3 get_u(vector<BOID>& a_flock, GOAL& a_goal, int flock_index);
float get_d(vector<BOID>& a_flock, GOAL& a_goal, int flock_index);
float flock_radius(vector<BOID>& a_flock, int flock_index);

glm::vec3 get_average_v(vector<BOID>& a_flock, int flock_index);
void init_a_flock(vector<BOID>& a_flock);
void apply_goal_attraction(vector<BOID>&a_flock, GOAL& a_goal);
void print_flock(vector<BOID>& a_flock);
#endif
