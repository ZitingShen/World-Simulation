#include "boid.h"

using namespace std;

BOID* new_boid(){
  BOID* a_boid = (BOID*)malloc(sizeof(BOID));
  a_boid->pos.reset();
  a_boid->velocity.reset();
  a_boid->velocity = SPAWN_VELOCITY;
  a_boid->partner_radius = PARTNER_RADIUS;
  a_boid->wing_rotation = rand()%(2*MAX_WING_ROTATION)
                          - MAX_WING_ROTATION;
  a_boid->wing_rotation *= DEGREE_TO_RADIAN;
  a_boid->flock_index = rand()%(DEFAULT_FLOCK_NUM);

  a_boid->pos = (a_boid->flock_index==0)?SPAWN_POSITION_I:SPAWN_POSITION_II;
  a_boid->wing_rotation_direction = 1;
  return a_boid;
}

BOID* new_boid(const vec4& velocity, float radius, const vec4& pos){
  BOID* a_boid = (BOID*)malloc(sizeof(BOID));
  a_boid->pos.reset();
  a_boid->velocity.reset();
  a_boid->pos = pos;
  a_boid->velocity = velocity;
  a_boid->partner_radius = radius;
  a_boid->wing_rotation = rand()%(2*MAX_WING_ROTATION)
                          - MAX_WING_ROTATION;
  a_boid->wing_rotation *= DEGREE_TO_RADIAN;
  a_boid->flock_index = rand()%(DEFAULT_FLOCK_NUM);
  a_boid->wing_rotation_direction = 1;
  return a_boid;
}

bool is_partner(BOID* source, BOID* target){
  return source->partner_radius >= distance(source->pos, target->pos);
}

void update_velocity(List* a_flock, GOAL* a_goal){
  if (a_flock == NULL || a_flock->length < 2) return;
  NODE* current_boid = a_flock->head;
  NODE* potential_partner;
  vec4 s_modifier(0, 0, 0, 0);
  vec4 a_modifier(0, 0, 0, 0);
  vec4 c_modifier(0, 0, 0, 0);
  vec4 f_modifier(0, 0, 0, 0);
  vec4 flock_center;
  int num_of_partners;
  int num_of_boids_other_flocks;
  bool close_to_goal = false;
  float dis_to_partner;

  BOID* source = NULL;
  BOID* target = NULL;
  while (current_boid != NULL){
    num_of_partners = 0; //reset for the next boid
    num_of_boids_other_flocks = 0;
    potential_partner = a_flock->head;
    num_of_partners = 0;
    close_to_goal = length(a_goal->pos - ((BOID*)(current_boid->data))->pos) < APPROACHING_GOAL;
    while (potential_partner != NULL){
      if (potential_partner == current_boid) {
        potential_partner = potential_partner->next;
        continue;
      }
      source = (BOID*)(current_boid->data);
      target = (BOID*)(potential_partner->data);
      if (is_partner(source, target)){
        if (target->flock_index == source->flock_index) {
          num_of_partners++;
          dis_to_partner = distance(source->pos, target->pos);
          if (dis_to_partner > SCATTERING){
            s_modifier += (source->pos - target->pos) * 0.95;
            a_modifier += target->velocity;
            c_modifier += target->pos * 1.05;
          }else if (dis_to_partner < COLLIDING){
            s_modifier += (source->pos - target->pos) * 1.05;
            a_modifier += target->velocity;
            c_modifier += target->pos * 0.95;
          }else{
            s_modifier += source->pos - target->pos;
            a_modifier += target->velocity;
            c_modifier += target->pos;
          }
          if(close_to_goal){// if close to goal, scatter
            //cout << "now near goal" << endl;
            s_modifier = s_modifier * 1.1;;
            a_modifier = a_modifier * 0.9;;
          } 
        } else {
          num_of_boids_other_flocks++;
          f_modifier += source->pos - target->pos;
        }
      }
      potential_partner = potential_partner->next;
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

    current_boid = current_boid->next;
  }
}

void update_pos(List* a_flock){
  if (a_flock == NULL || a_flock->length == 0) return;
  BOID* a_boid;
  NODE* current = a_flock->head;
  while (current != NULL){
    a_boid = (BOID*)(current->data);
    a_boid->pos += a_boid->velocity;
    current = current->next;
  }
}

void update_wing_rotation(List* a_flock){
  if (a_flock == NULL || a_flock->length == 0) return;
  BOID* a_boid;
  NODE* current = a_flock->head;
  while (current != NULL){
    a_boid = (BOID*)(current->data);
    if (a_boid->wing_rotation > MAX_WING_ROTATION*DEGREE_TO_RADIAN ||
        a_boid->wing_rotation < -MAX_WING_ROTATION*DEGREE_TO_RADIAN)
      a_boid->wing_rotation_direction *= -1;
    a_boid->wing_rotation += a_boid->wing_rotation_direction* WING_ROTATION_PER_FRAME;
    current = current->next;
  }
}

vec4 flock_centroid(List* a_flock, int flock_index){
  if (a_flock == NULL || a_flock->length == 0)
    return vec4(0, 0, 0, 1);
  NODE* current = a_flock->head;
  vec4 centroid(0, 0, 0, 1);
  
  int counter = 0;
  while (current != NULL){
    if (((BOID*)(current->data))->flock_index != flock_index) {
      current = current->next;
      continue;
    }
    centroid += ((BOID*)(current->data))->pos;
    counter++;
    current = current->next;
  }
  return centroid*(1.0f/counter);
}

vec4 mid_point(List* a_flock, GOAL* a_goal, int flock_index){
  if (a_flock == NULL || a_flock->length == 0)
    return vec4(0, 0, 0, 1);
  return (flock_centroid(a_flock, flock_index)+(a_goal->pos))*(0.5f);
}

vec4 get_u(List* a_flock, GOAL* a_goal, int flock_index){
  if (a_flock == NULL || a_flock->length == 0)
    return vec4(0, 0, 0, 0);
  return (a_goal->pos - flock_centroid(a_flock, flock_index));
}

float get_d(List* a_flock, GOAL* a_goal, int flock_index){
  if (a_flock == NULL || a_flock->length == 0)
    return 0;
  return distance(flock_centroid(a_flock, flock_index), a_goal->pos);
}

vec4 get_average_v(List* a_flock, int flock_index){
  NODE* current=a_flock->head;
  BOID* a_boid = NULL;
  int count = 0;
  vec4 average_v(0,0,0,0);
  while(current!= NULL){
    a_boid = (BOID*)(current->data);
    if (a_boid->flock_index == flock_index){
      average_v += a_boid->velocity;
      count++;
    }
    current = current->next;
  }
  average_v = average_v * (1.0f / count);
  return average_v;
}

float flock_radius(List* a_flock, int flock_index){
  if (a_flock == NULL || a_flock->length == 0)
    return 0;
  float max_r = 0;
  float dis   = 0;
  NODE* current = a_flock->head;
  vec4 centroid = flock_centroid(a_flock, flock_index);
  while (current != NULL){
    dis = distance(((BOID*) (current->data))->pos, centroid);
    max_r = max_r < dis ? dis : max_r;
    current = current->next;
  }
  return max_r;
}

void add_a_boid(List* a_flock){
  if (a_flock == NULL){return;}  // use init_a_flock to create a new flock
  int default_cube_length = PARTNER_RADIUS*sqrt(2);
  int half_cube_length    = default_cube_length/2;
  vec4 pos;
  if (a_flock->length == 0) {
    vec4 pos;
    pos[0] = (rand() % default_cube_length) - half_cube_length;
    pos[1] = (rand() % default_cube_length) - half_cube_length;
    pos[2] = (rand() % default_cube_length) - half_cube_length;
    pos[3] = 1;
    list_insert(a_flock, new_boid(SPAWN_VELOCITY, PARTNER_RADIUS, pos), 0);
    return;
  }

  BOID* target = (BOID*)list_get(a_flock, rand() % a_flock->length);
  // spawning within the partner radius of the target
  pos[0] = target->pos[0] + (rand() % default_cube_length) - half_cube_length;
  pos[1] = target->pos[1] + (rand() % default_cube_length) - half_cube_length;
  pos[2] = target->pos[2] + (rand() % default_cube_length) - half_cube_length;
  pos[3] = 1;

  list_insert(a_flock, new_boid(target->velocity, PARTNER_RADIUS, pos), 0);
}

void remove_a_boid(List* a_flock){
  if (a_flock == NULL || a_flock->length == 0) return; //nothing to remove
  list_delete(a_flock, rand() % a_flock->length);
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

void apply_goal_attraction(List* a_flock, GOAL* a_goal){
  NODE* current=a_flock->head;
  vec4 v_modifier(0, 0, 0, 0);
  float dis_to_goal;
  float max_attraction;
  BOID* a_boid = NULL;
  while (current!=NULL){
    a_boid = (BOID*)(current->data);
    v_modifier = a_goal->pos - a_boid->pos;
    dis_to_goal = length(v_modifier);
    max_attraction = 0.6*length(a_boid->velocity);

    v_modifier = v_modifier*ATTRACTION_WEIGHT;
    if (APPROACHING_GOAL<dis_to_goal){ // not near the goal
      a_boid->velocity += v_modifier;
    }else{ // near goal scenario
      /* Let's slow down */
      if (length(a_boid->velocity) > 3.0*length(a_goal->velocity) 
          && length(a_boid->velocity) > BOID_SPEED_FLOOR
          ){
        //cout << length(a_boid->velocity) << endl;
        //cout << "slowing down" << endl;
        a_boid->velocity = a_boid->velocity * 0.95;
        a_boid->velocity += v_modifier;
      }
    }
    if (length(a_boid->velocity) > 4.0*length(a_goal->velocity)){
      a_boid->velocity += v_modifier;
      //cout << "applying absolute cap" << endl;
      if(length(a_boid->velocity) > BOID_SPEED_FLOOR){
        a_boid->velocity = normalise(a_boid->velocity) * 4.0*length(a_goal->velocity);
      }
    }
    current = current->next; 
  }
}

void print_flock(List* a_flock) {
  for (int i = 0; i < DEFAULT_FLOCK_NUM; i++) {
    vec4 centroid = flock_centroid(a_flock, i);
    cout << "Flock" << i << "'s centroid: " << centroid[0] << ", " 
    << centroid[1] << ", " << centroid[2] << endl;
    float radius = flock_radius(a_flock, i);
    cout << "Flock" << i << "'s radius: " << radius << endl;
    cout << endl;
  }
}

PREDATOR* create_a_predator(List* a_flock, GOAL* a_goal, bool& guardian){
  if (guardian){ // only create if only already exists
    return NULL;
  }
  guardian = true;
  PREDATOR* a_predator = new PREDATOR;
  a_predator->pos = (a_goal->pos + (flock_centroid(a_flock, 0) + flock_centroid(a_flock, 1)) * 0.5) * 0.5;
  a_predator->velocity = a_goal->velocity;
  a_predator->deterrence_range = 5000;
  return a_predator;
}

void move_predator(List* a_flock, PREDATOR* a_predator, GOAL* a_goal, bool& guardian){ // orbiting the goal
  if (!guardian){
    return;
  }  
  NODE* current=a_flock->head;
  BOID* a_boid;
  vec4 target_pos;
  float nearest = std::numeric_limits<float>::max();
  while(current != NULL){
    a_boid = (BOID*)(current->data);
    if (distance(a_predator->pos, a_boid->pos) < nearest){
      nearest = distance(a_predator->pos, a_boid->pos);
      target_pos = a_boid->pos;
    }
    current = current->next;
  }

  vec4 acceleration = target_pos - a_predator->pos;
  a_predator->velocity += (acceleration * 0.008);

  if (length(a_predator->velocity) > PREDATOR_SPEED_CAP){
    a_predator->velocity = normalise(a_predator->velocity) * PREDATOR_SPEED_CAP;
  }

  a_predator->pos += a_predator->velocity;
  //cout << length(a_predator->velocity) << endl;
}

void apply_predator_deterrence(List* a_flock, PREDATOR* a_predator, bool& guardian){
  if (!guardian){
    return;
  }
  NODE* current=a_flock->head;
  BOID* a_boid = NULL;
  vec4 dis_to_predator;
  while (current!=NULL){
    a_boid = (BOID*)current->data;
    if (distance(a_boid->pos, a_predator->pos) < a_predator->deterrence_range){
      dis_to_predator = a_boid->pos - a_predator->pos;
      a_boid->velocity += dis_to_predator * DETERRENCE_WEIGHT;
    }
    current = current->next;
  }
}

void delete_predator(PREDATOR* a_predator, bool& guardian){
  guardian = false;
  delete a_predator;
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

void draw_a_predator(){
  if (!GUARDIAN){
    return;
  }
  glUseProgram(boid_program);
  glBindVertexArray(vao_goal);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo_goal_indices);
  GLfloat trans[16];
  memcpy(trans, MV_MAT, sizeof(GLfloat)*16);
  myTranslate(trans, A_PREDATOR->pos[0], A_PREDATOR->pos[1], A_PREDATOR->pos[2]);
  glUniformMatrix4fv(proj, 1, GL_FALSE, PROJ_MAT);
  glUniformMatrix4fv(mv, 1, GL_FALSE, trans);
  glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_BYTE, (void*) 0);
}