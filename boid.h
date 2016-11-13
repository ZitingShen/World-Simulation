#ifndef BOID_H 
#define BOID_H

#include "goal.h"
#include "fin.h"
#include "common.h"
#include "read_ppm.h"
#include "glm/gtx/transform.hpp"
#include "glm/gtx/vector_angle.hpp"
#include <time.h>

const GLfloat A_BOID[][3]=
            {{0, 0, 0}, // position of the centroid
             {BOID_SIZE, -BOID_SIZE, 0}, // position of the right vertex
             {0, BOID_SIZE*2, 0}, // position of the head
             {-BOID_SIZE, -BOID_SIZE, 0}}; // position of the left vertex

const GLfloat A_BOID_TEX[][2] = {{0.5, 0.3}, {1, 0}, {0.5, 1}, {0.0}};

const GLuint BOID_INDEX[] = {0, 1, 2, 0, 2, 3};

const glm::vec3 SPAWN_POSITION_I = glm::vec3(1000.0f, 1000.0f, 3000.0f);
const glm::vec3 SPAWN_POSITION_II = glm::vec3(-1000.0f, -1000.0f, 3000.0f);
 //initial speed parallel with y-axis
const glm::vec3 SPAWN_VELOCITY = glm::vec3(0.0f, 0.01f, 0.0f);

const glm::vec3 centroid_init = glm::vec3(0.0f, 0.0f, 0.0f);
const glm::vec3 head_init = glm::vec3(0.0f, BOID_SIZE*2, 0.0f);
const glm::vec3 left_init = glm::vec3(-BOID_SIZE, -BOID_SIZE, 0.0f);
const glm::vec3 right_init = glm::vec3(BOID_SIZE, -BOID_SIZE, 0.0f);

/* Dynamically dealing with Weights */
const float SCATTERING = 0.8f * PARTNER_RADIUS; // too far away from partner
const float COLLIDING  = 0.2f * PARTNER_RADIUS; // too close to partner
const float FLOCK_RAIUS_CAP = 10.0f * BOID_SIZE;
const float APPROACHING_GOAL = 20.0f * GOAL_SIZE;
const float Z_SPEED_CAP = 20.0f;
const float BOID_SPEED_FLOOR = 55.0f;
const float PREDATOR_SPEED_CAP = 300.0f;

class BOID{
  public:
    glm::vec3 pos;
    glm::vec3 velocity;   
    float partner_radius;// the radius within which it looks for partners
    
    BOID();
};

bool is_partner(BOID& source, BOID& target);
void update_velocity(vector<BOID>& a_flock, GOAL& a_goal);
void update_pos(vector<BOID>& a_flock);

glm::vec3 flock_centroid(vector<BOID>& a_flock);
glm::vec3 mid_point(vector<BOID>& a_flock, GOAL& a_goal);
glm::vec3 get_u(vector<BOID>& a_flock, GOAL& a_goal);
float get_d(vector<BOID>& a_flock, GOAL& a_goal);
float flock_radius(vector<BOID>& a_flock);

glm::vec3 get_average_v(vector<BOID>& a_flock);
void init_a_flock(vector<BOID>& a_flock);
void apply_goal_attraction(vector<BOID>&a_flock, GOAL& a_goal);
void print_flock(vector<BOID>& a_flock);
void init_flock_mesh(MESH& mesh, GLuint shader);
void draw_a_flock(vector<BOID>& a_flock, MESH& mesh, GLuint shader, 
  glm::mat4& PROJ_MAT, glm::mat4 MV_MAT, LIGHT THE_LIGHT);
#endif