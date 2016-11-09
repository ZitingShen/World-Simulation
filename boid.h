#ifndef BOID_H 
#define BOID_H

#include "list.h"
#include "goal.h"
#include "common.h"
#include <time.h>
#include <limits>
#include <algorithm>

const vec4 SPAWN_POSITION_I(1000.0, 1000.0, 3000.0, 1);
const vec4 SPAWN_POSITION_II(-1000.0, -1000.0, 3000.0, 1);
 //initial speed parallel with y-axis
const vec4 SPAWN_VELOCITY(0, 0.01, 0, 0);
const vec4 EMPTY_POS(0, 0, 0, 1);

const vec3 centroid_init(0, 0, 0);
const vec3 head_init(0, BOID_SIZE*2, 0);
const vec3 left_init(-BOID_SIZE, -BOID_SIZE, 0);
const vec3 right_init(BOID_SIZE, -BOID_SIZE, 0);

/* Dynamically dealing with Weights */
const float SCATTERING = 0.8 * PARTNER_RADIUS; // too far away from partner
const float COLLIDING  = 0.2 * PARTNER_RADIUS; // too close to partner
const float FLOCK_RAIUS_CAP = 10 * BOID_SIZE;
const float APPROACHING_GOAL = GOAL_SIZE * 20.0;
const float Z_SPEED_CAP = 20;
const float BOID_SPEED_FLOOR = 55;
const float PREDATOR_SPEED_CAP = 300;

typedef struct _boid{
  GLfloat wing_rotation;          // for flapping extra credit
  int wing_rotation_direction;    // 1 for downwards, 0 for upwards
  int flock_index;  
  vec4 pos;
  vec4 velocity;             // also determines PA direction; and the degrees of rotation   
  float partner_radius;           // the radius within which it looks for partners

  boid();
  new_boid(const vec4& velocity, float radius, const vec4& pos);
} BOID;

typedef struct _predator{
  vec4 pos;
  vec4 velocity;
  float deterrence_range;
} PREDATOR;

bool is_partner(BOID* source, BOID* target);
void update_velocity(List* a_flock, GOAL* a_goal);
void update_pos(List* a_flock);
void update_wing_rotation(List* a_flock);

vec4 get_current_pos(BOID* a_boid);
vec4 flock_centroid(List* a_flock, int flock_index);
vec4 mid_point(List* a_flock, GOAL* a_goal, int flock_index);
vec4 get_u(List* a_flock, GOAL* a_goal, int flock_index);
float get_d(List* a_flock, GOAL* a_goal, int flock_index);
float flock_radius(List* a_flock, int flock_index);

vec4 get_average_v(List* a_flock, int flock_index);

void add_a_boid(List* a_flock);
void remove_a_boid(List* a_flock);

void init_a_flock(List* a_flock);

void apply_goal_attraction(List* a_flock, GOAL* a_goal);

void print_flock(List* a_flock);
//void update_rotation(BOID* a_boid);
/* To DO */

PREDATOR* create_a_predator(List* a_flock, GOAL* a_goal, bool& guardian);
void move_predator(List* a_flock, PREDATOR* a_predator, GOAL* a_goal, bool& guardian);
void draw_predator(PREDATOR* a_predator, bool& guardian, GLfloat mv_mat[]);
void apply_predator_deterrence(List* a_flock, PREDATOR* a_predator, bool& guardian);
void delete_predator(PREDATOR* a_predator, bool& guardian);

#endif
