#include "boid.h"

using namespace std;

BOID::BOID _boid(){
  this->pos = (a_boid->flock_index==0)?SPAWN_POSITION_I:SPAWN_POSITION_II;
  this->velocity = SPAWN_VELOCITY;
  this->partner_radius = PARTNER_RADIUS;
  this->flock_index = rand()%(DEFAULT_FLOCK_NUM);
}

bool is_partner(BOID* source, BOID* target){
  return source->partner_radius >= glm::distance(source->pos, target->pos);
}

void update_velocity(vector<BOID>& a_flock, GOAL& a_goal){
  if (a_flock.size < 2) return;
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
    close_to_goal = glm::length(a_goal.pos - source.pos) < APPROACHING_GOAL;
    for (auto target = a_flock.begin(); target != a_flock.end(); target++) {
      if (potential_partner == current_boid) continue;
      if (is_partner(source, target)){
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
      s_modifier = s_modifier*(SEPARATION_WEIGHT/(float)num_of_partners)*1.8;
      a_modifier = (a_modifier*(1/(float)num_of_partners) - source->velocity)*ALIGNMENT_WEIGHT*0.8;
      c_modifier = (c_modifier*(1/(float)num_of_partners) - source->pos)*COHESION_WEIGHT;
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
    a_boid->pos[0] += (rand() % default_cube_length) - half_cube_length;
    a_boid->pos[1] += (rand() % default_cube_length) - half_cube_length;
    a_boid->pos[2] += (rand() % default_cube_length) - half_cube_length;
    a_flock.add(a_boid);
  }
}

void apply_goal_attraction(vector<BOID> a_flock, GOAL& a_goal){
  glm::vec3 v_modifier(0, 0, 0);
  float dis_to_goal;
  float max_attraction;
  float boid_speed, goal_speed = glm::length(a_goal.velocity);
  for (auto a_boid = a_flock.begin(); a_boid != a_flock.end(); a_boid++) {
    v_modifier = a_goal->pos - a_boid->pos;
    dis_to_goal = glm::length(v_modifier);
    max_attraction = 0.6*glm::length(a_boid->velocity);
    boid_speed = glm::length(a_boid->velocity);
    v_modifier = v_modifier*ATTRACTION_WEIGHT;
    if (APPROACHING_GOAL < dis_to_goal) { // not near the goal
      a_boid->velocity += v_modifier;
    }else if (speed > 3*goal_speed && boid_speed > BOID_SPEED_FLOOR){ // near goal scenario
      a_boid->velocity = a_boid->velocity * 0.95;
      a_boid->velocity += v_modifier;
    }
    if (boid_speed > 4.0*goal_speed){ //applying absolute cap;
      a_boid->velocity += v_modifier;
      
      if(glm::length(a_boid->velocity) > BOID_SPEED_FLOOR){
        a_boid->velocity = glm::normalise(a_boid->velocity)*4*goal_speed;
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

void draw_a_flock(){
  if (A_FLOCK == NULL) return;
  NODE* current = A_FLOCK->head;
  BOID* some_boid = NULL;
  for (int i = 0; i < A_FLOCK->length; i++){
    some_boid = (BOID*)(current->data);
    vec3 velocity3 = normalise(reduce(some_boid->velocity));
    vec3 initial3 = normalise(reduce(SPAWN_VELOCITY));
    vec3 rotate_normal = normalise(cross(velocity3, initial3));
    float angle = oriented_angle(initial3, velocity3, 
                                rotate_normal);
    float shades_angle = oriented_angle(initial3, velocity3, 
                                       vec3(0, 0, 1)); 
    GLfloat trans[16];

    glUseProgram(boid_program);
    glBindVertexArray(vao_left);
    glUniformMatrix4fv(proj, 1, GL_FALSE, PROJ_MAT);
    
    if (some_boid->flock_index == 0) {
      glBindBuffer(GL_ARRAY_BUFFER, vbo_color_i_left);
      color = glGetAttribLocation(boid_program, "vColor");
      glEnableVertexAttribArray(color);
      glVertexAttribPointer(color, 3, GL_FLOAT, GL_FALSE, 0, (void*) 0);
    } else {
      glBindBuffer(GL_ARRAY_BUFFER, vbo_color_ii_left);
      color = glGetAttribLocation(boid_program, "vColor");
      glEnableVertexAttribArray(color);
      glVertexAttribPointer(color, 3, GL_FLOAT, GL_FALSE, 0, (void*) 0);
    }
    
    memcpy(trans, MV_MAT, sizeof(GLfloat)*16);
    myTranslate(trans, some_boid->pos[0], some_boid->pos[1], some_boid->pos[2]);
    myRotate(trans, angle, rotate_normal[0], rotate_normal[1], rotate_normal[2]);
    myRotate(trans, -some_boid->wing_rotation, 0, 1, 0);
    glUniformMatrix4fv(mv, 1, GL_FALSE, trans);
    glDrawArrays(GL_TRIANGLES, 0, 3);
   
    glUseProgram(shades_program);
    glBindVertexArray(shades_vao_left);
    glUniformMatrix4fv(shades_proj, 1, GL_FALSE, PROJ_MAT);
    memcpy(trans, MV_MAT, sizeof(GLfloat)*16);
    myTranslate(trans, some_boid->pos[0], some_boid->pos[1], SHADES_HEIGHT);
    myRotate(trans, shades_angle, 0, 0, 1);
    glUniformMatrix4fv(shades_mv, 1, GL_FALSE, trans);
    glDrawArrays(GL_TRIANGLES, 0, 3);
   
    current = current->next;
  }

  current = A_FLOCK->head;
  for (int i = 0; i < A_FLOCK->length; i++){
    some_boid = (BOID*)(current->data);
    vec3 velocity3 = normalise(reduce(some_boid->velocity));
    vec3 initial3 = normalise(reduce(SPAWN_VELOCITY));
    vec3 rotate_normal = normalise(cross(velocity3, initial3));
    float angle = oriented_angle(initial3, velocity3, 
                                rotate_normal);
    float shades_angle = oriented_angle(initial3, velocity3, 
                                       vec3(0, 0, 1));
    GLfloat trans[16];

    glUseProgram(boid_program);
    glBindVertexArray(vao_right);
    glUniformMatrix4fv(proj, 1, GL_FALSE, PROJ_MAT);
    if (some_boid->flock_index == 0) {
      glBindBuffer(GL_ARRAY_BUFFER, vbo_color_i_right);
      color = glGetAttribLocation(boid_program, "vColor");
      glEnableVertexAttribArray(color);
      glVertexAttribPointer(color, 3, GL_FLOAT, GL_FALSE, 0, (void*) 0);
    } else {
      glBindBuffer(GL_ARRAY_BUFFER, vbo_color_ii_right);
      color = glGetAttribLocation(boid_program, "vColor");
      glEnableVertexAttribArray(color);
      glVertexAttribPointer(color, 3, GL_FLOAT, GL_FALSE, 0, (void*) 0);
    }
    memcpy(trans, MV_MAT, sizeof(GLfloat)*16);
    myTranslate(trans, some_boid->pos[0], some_boid->pos[1], some_boid->pos[2]);
    myRotate(trans, angle, rotate_normal[0], rotate_normal[1], rotate_normal[2]);
    myRotate(trans, some_boid->wing_rotation, 0, 1, 0);
    glUniformMatrix4fv(mv, 1, GL_FALSE, trans);
    glDrawArrays(GL_TRIANGLES, 0, 3);

    glUseProgram(shades_program);
    glBindVertexArray(shades_vao_right);
    glUniformMatrix4fv(shades_proj, 1, GL_FALSE, PROJ_MAT);
    memcpy(trans, MV_MAT, sizeof(GLfloat)*16);
    myTranslate(trans, some_boid->pos[0], some_boid->pos[1], SHADES_HEIGHT);
    myRotate(trans, shades_angle, 0, 0, 1);
    glUniformMatrix4fv(shades_mv, 1, GL_FALSE, trans);
    glDrawArrays(GL_TRIANGLES, 0, 3);
    current = current->next;
  }
  
}