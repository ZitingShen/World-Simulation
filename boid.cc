#include "boid.h"

using namespace std;

BOID::_boid(){
  pos = (flock_index==0)?SPAWN_POSITION_I:SPAWN_POSITION_II;
  velocity = SPAWN_VELOCITY;
  partner_radius = PARTNER_RADIUS;
  flock_index = rand()%(DEFAULT_FLOCK_NUM);
}

bool is_partner(BOID& source, BOID& target){
  return source.partner_radius >= glm::distance(source.pos, 
    target.pos);
}

void update_velocity(vector<BOID>& a_flock, GOAL& a_goal){
  if (a_flock.size() < 2) return;
  glm::vec3 s_modifier, a_modifier, c_modifier, f_modifier;
  glm::vec3 flock_center;
  int num_of_partners;
  int num_of_boids_other_flocks;
  bool close_to_goal = false;
  float dis_to_partner;

  for (auto source = a_flock.begin(); source != a_flock.end(); source++) {
    num_of_partners = 0; //reset for the next boid
    num_of_boids_other_flocks = 0;
    num_of_partners = 0;
    close_to_goal = glm::length(a_goal.pos - source->pos) < APPROACHING_GOAL;
    for (auto target = a_flock.begin(); target != a_flock.end(); target++) {
      if (target == source) continue;
      if (is_partner(*source, *target)){
        if (target->flock_index == source->flock_index) {
          num_of_partners++;
          dis_to_partner = distance(source->pos, target->pos);
          if (dis_to_partner > SCATTERING){
            s_modifier += (source->pos - target->pos) * 0.95f;
            a_modifier += target->velocity;
            c_modifier += target->pos * 1.05f;
          }else if (dis_to_partner < COLLIDING){
            s_modifier += (source->pos - target->pos) * 1.05f;
            a_modifier += target->velocity;
            c_modifier += target->pos * 0.95f;
          }else{
            s_modifier += source->pos - target->pos;
            a_modifier += target->velocity;
            c_modifier += target->pos;
          }
          if(close_to_goal){// if close to goal, scatter
            //cout << "now near goal" << endl;
            s_modifier = s_modifier * 1.1f;;
            a_modifier = a_modifier * 0.9f;
          } 
        } else {
          num_of_boids_other_flocks++;
          f_modifier += source->pos - target->pos;
        }
      }
    }
    if (num_of_partners != 0) {
      //cout << "num_of_partners = " << num_of_partners << endl;
      s_modifier = s_modifier*(SEPARATION_WEIGHT/(float)num_of_partners)*1.8f;
      a_modifier = (a_modifier*(1/(float)num_of_partners) 
        - source->velocity)*ALIGNMENT_WEIGHT*0.8f;
      c_modifier = (c_modifier*(1/(float)num_of_partners) 
        - source->pos)*COHESION_WEIGHT;
      source->velocity += s_modifier + a_modifier + c_modifier;
    }
    if (num_of_boids_other_flocks != 0) {
      f_modifier = f_modifier*(DETERRENCE_WEIGHT/(float)num_of_boids_other_flocks);
      source->velocity += f_modifier;
    }

    flock_center = flock_centroid(a_flock, source->flock_index);
    if (distance(source->pos, flock_center) > FLOCK_RAIUS_CAP){
      source->velocity += (flock_center - source->pos) * STAY_IN_FLOCK_WEIGHT;
    }

    source->velocity[2] = max(-Z_SPEED_CAP, min(source->velocity[2], Z_SPEED_CAP));
  }
}

void update_pos(vector<BOID>& a_flock){
  if (a_flock.size() == 0) return;
  for (auto a_boid = a_flock.begin(); a_boid != a_flock.end(); a_boid++) {
    a_boid->pos += a_boid->velocity;
  }
}

glm::vec3 flock_centroid(vector<BOID>& a_flock, int flock_index){
  glm::vec3 centroid = glm::vec3(0, 0, 0);
  if (a_flock.size() == 0)
    return centroid;
  
  int counter = 0;
  for (auto current = a_flock.begin(); current != a_flock.end(); current++) {
    if (current->flock_index != flock_index) continue;
    centroid += current->pos;
    counter++;
  }
  return centroid*(1.0f/counter);
}

glm::vec3 mid_point(vector<BOID>& a_flock, GOAL& a_goal, int flock_index){
  if (a_flock.size() == 0)
    return glm::vec3(0, 0, 0);
  return (flock_centroid(a_flock, flock_index)+(a_goal.pos))*(0.5f);
}

glm::vec3 get_u(vector<BOID>& a_flock, GOAL& a_goal, int flock_index){
  if (a_flock.size() == 0)
    return glm::vec3(0, 0, 0);
  return (a_goal.pos - flock_centroid(a_flock, flock_index));
}

float get_d(vector<BOID>& a_flock, GOAL& a_goal, int flock_index){
  if (a_flock.size() == 0)
    return 0;
  return glm::distance(flock_centroid(a_flock, flock_index), a_goal.pos);
}

glm::vec3 get_average_v(vector<BOID>& a_flock, int flock_index){
  int count = 0;
  glm::vec3 average_v = glm::vec3(0, 0, 0);
  for (auto a_boid = a_flock.begin(); a_boid != a_flock.end(); a_boid++) {
    if (a_boid->flock_index == flock_index){
      average_v += a_boid->velocity;
      count++;
    }
  }
  average_v = average_v * (1.0f / count);
  return average_v;
}

float flock_radius(vector<BOID>& a_flock, int flock_index){
  if (a_flock.size() == 0) return 0;
  float max_r = 0;
  float dis   = 0;
  glm::vec3 centroid = flock_centroid(a_flock, flock_index);
  for (auto current = a_flock.begin(); current != a_flock.end(); current++) {
    dis = distance(current->pos, centroid);
    max_r = max_r < dis ? dis : max_r;
  }
  return max_r;
}

void init_a_flock(vector<BOID>& a_flock){
  int default_cube_length = SPAWN_CUBE_LENGTH*SQRT_2;
  int half_cube_length = default_cube_length*0.5f;

  for (int i = 0; i < DEFAULT_FLOCK_SIZE; i++){
    BOID a_boid;
    a_boid.pos[0] += (rand() % default_cube_length) - half_cube_length;
    a_boid.pos[1] += (rand() % default_cube_length) - half_cube_length;
    a_boid.pos[2] += (rand() % default_cube_length) - half_cube_length;
    a_flock.push_back(a_boid);
  }
}

void apply_goal_attraction(vector<BOID>& a_flock, GOAL& a_goal){
  glm::vec3 v_modifier(0, 0, 0);
  float dis_to_goal;
  float boid_speed, goal_speed = glm::length(a_goal.velocity);
  for (auto a_boid = a_flock.begin(); a_boid != a_flock.end(); a_boid++) {
    v_modifier = a_goal.pos - a_boid->pos;
    dis_to_goal = glm::length(v_modifier);
    boid_speed = glm::length(a_boid->velocity);
    v_modifier = v_modifier*ATTRACTION_WEIGHT;
    if (APPROACHING_GOAL < dis_to_goal) { // not near the goal
      a_boid->velocity += v_modifier;
    }else if (boid_speed > 3*goal_speed 
    && boid_speed > BOID_SPEED_FLOOR){ // near goal scenario
      a_boid->velocity = a_boid->velocity * 0.95f;
      a_boid->velocity += v_modifier;
    }
    if (boid_speed > 4.0*goal_speed){ //applying absolute cap;
      a_boid->velocity += v_modifier;
      
      if(glm::length(a_boid->velocity) > BOID_SPEED_FLOOR){
        a_boid->velocity = glm::normalize(a_boid->velocity)*goal_speed*4.0f;
      }
    }
  }
}

void print_flock(vector<BOID>& a_flock) {
  for (int i = 0; i < DEFAULT_FLOCK_NUM; i++) {
    glm::vec3 centroid = flock_centroid(a_flock, i);
    cout << "Flock" << i << "'s centroid: " << centroid[0] << ", " 
    << centroid[1] << ", " << centroid[2] << endl;
    float radius = flock_radius(a_flock, i);
    cout << "Flock" << i << "'s radius: " << radius << endl;
    cout << endl;
  }
}

void init_flock_mesh(MESH& mesh, GLuint shader) {
  mesh.num_v = 4;
  mesh.num_f = 2;
  mesh.vertices.resize(4);
  for (int i = 0; i< 4; i++) {
    mesh.vertices[i].pos = glm::vec3(A_BOID[i][0], A_BOID[i][1], 
      A_BOID[i][2]);  
    mesh.vertices[i].tex_coords = glm::vec2(A_BOID_TEX[i][0], 
      A_BOID_TEX[i][1]);
  }
  for (int i = 0; i < 6; i++)
    mesh.faces.draw_indices.push_back(BOID_INDEX[i]);
  mesh.texels.resize(1);
  if (!read_ppm("ppms/paper.ppm", &mesh.texels[0])) {
    cerr << "Fail loading texture: ppms/paper.ppm" << endl;
  }
  mesh.compute_face_normal();
  mesh.compute_vertex_normal();
  mesh.setup(shader);
}

void draw_a_flock(vector<BOID>& a_flock, MESH& mesh, GLuint shader, 
  glm::mat4& PROJ_MAT, glm::mat4 MV_MAT, LIGHT THE_LIGHT){
  for (unsigned int i = 0; i < a_flock.size(); i++){
    glm::vec3 rotate_normal = glm::normalize(glm::cross(a_flock[i].velocity, 
      SPAWN_VELOCITY));
    float angle = glm::orientedAngle(SPAWN_VELOCITY, a_flock[i].velocity, 
                                rotate_normal);
    THE_LIGHT.light0 = THE_LIGHT.light0*MV_MAT;
    MV_MAT = glm::translate(MV_MAT, a_flock[i].pos);
    MV_MAT = glm::rotate(MV_MAT, angle, rotate_normal);
    mesh.draw(shader, PROJ_MAT, MV_MAT, THE_LIGHT);
  }
}