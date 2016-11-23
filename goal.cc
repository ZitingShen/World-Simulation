#include "goal.h"

using namespace std;

GOAL::GOAL(){
  this->pos = DEFAULT_GOAL_SPAWN_POSITION;
  this->planet =
  this->velocity = DEFAULT_GOAL_SPAWN_VELOCITY;

  this->MOVE_ALONG_X_NEGATIVE = false;
  this->MOVE_ALONG_X_POSITIVE = false;
  this->MOVE_ALONG_Y_NEGATIVE = false;
  this->MOVE_ALONG_Y_POSITIVE = false;
  this->ACCELERATE = false;
  this->DECELERATE = false;

  this->steer = false;
}

void GOAL::toggle_steer(){
  this->steer = !this->steer;
}

void update_goal_velocity(GOAL& goal){
  //float temp_z;
  if (!goal.steer){
    goal.planet[0] += GOAL_MOVEMENT;
    goal.planet[1] += GOAL_MOVEMENT;
    goal.pos[0] = GOAL_RADIUS*glm::cos(goal.planet[0]);
    goal.pos[1] = GOAL_RADIUS*glm::sin(goal.planet[1]);
    //cout << "not steering" << endl;
  }else{
    //cout << "steering" << endl;
  if (goal.MOVE_ALONG_X_POSITIVE){
    if (goal.velocity[0] < 0){
      goal.velocity[0] = -goal.velocity[0];
    }
    goal.velocity[0] += DEFAULT_ACCELERATION_MARGIN;
  }

  if (goal.MOVE_ALONG_X_NEGATIVE){
    if (goal.velocity[0] > 0){
      goal.velocity[0] = -goal.velocity[0];
    }
    goal.velocity[0] -= DEFAULT_ACCELERATION_MARGIN;
  }

  if (goal.MOVE_ALONG_Y_POSITIVE){
    if (goal.velocity[1] < 0){
      goal.velocity[1] = -goal.velocity[1];
    }
    goal.velocity[1] += DEFAULT_ACCELERATION_MARGIN;
  }

  if (goal.MOVE_ALONG_Y_NEGATIVE){
    if (goal.velocity[1] > 0){
      goal.velocity[1] = -goal.velocity[1];
    }
    goal.velocity[1] -= DEFAULT_ACCELERATION_MARGIN;
  }

  if (goal.ACCELERATE){
    goal.velocity = goal.velocity*float(1 + DEFAULT_ACCELERATION_FACTOR);
    goal.velocity[2] = 0.01f; // do not accerlate on Z
  }

  if (goal.DECELERATE){
    goal.velocity =  goal.velocity*float(1 - DEFAULT_ACCELERATION_FACTOR);
    goal.velocity[2] = 0.01f; // do not accerlate on Z
  }
  }
}

void update_goal_pos(GOAL& goal){
  goal.pos += goal.velocity;
}

void teleport_goal(GOAL& goal){
  goal.pos[0] += rand() % 5000 - 10000;
  goal.pos[1] += rand() % 5000 - 10000;
}

void print_goal(GOAL& goal) {
  cout << "The goal's position: " << goal.pos[0] << ", " 
  << goal.pos[1] << ", "  << goal.pos[2] << endl;
  cout << "The goal's velocity: " << goal.velocity[0] << ", " 
  << goal.velocity[1] << ", "  << goal.velocity[2] << endl;
  cout << endl;
}

void init_goal_mesh(MESH& mesh, GLuint shader, glm::mat4& PROJ_MAT) {
  mesh.num_v = 8;
  mesh.num_f = 12;
  mesh.vertices.resize(8);
  for (int i = 0; i< 8; i++) {
    mesh.vertices[i].pos = glm::vec3(CUBE_VERTICES[i][0], 
      CUBE_VERTICES[i][1], CUBE_VERTICES[i][2]);
    mesh.vertices[i].tex_coords = glm::vec2(A_GOAL_TEX[i][0],
      A_GOAL_TEX[i][1]);
  }
  for (int i = 0; i < 36; i++)
    mesh.faces.draw_indices.push_back(CUBE_INDICES[i]);
  mesh.texels.resize(1);
  if (!read_ppm(GOAL_TEXTURE, &mesh.texels[0])) {
    cerr << "GOAL_MESH: FAILED TO LOAD TEXTURE" << endl;
  }
  mesh.compute_face_normal();
  mesh.compute_vertex_normal();
  mesh.setup(shader, PROJ_MAT);
}

void draw_a_goal(GOAL& goal, MESH& mesh, GLuint shader, glm::mat4& MV_MAT, LIGHT THE_LIGHT,
                 spotlight SPOT_LIGHT){
  glm::mat4 new_mv = MV_MAT;
  THE_LIGHT.light0 = THE_LIGHT.light0*MV_MAT;
  SPOT_LIGHT.pos = SPOT_LIGHT.pos*MV_MAT;
  new_mv = glm::translate(new_mv, goal.pos);

  new_mv = glm::translate(new_mv, glm::vec3(goal.pos[0],
                                            goal.pos[1],
                                            goal.pos[2])-mesh.center);
  cout << goal.pos[0] << " " << goal.pos[1] << " " << goal.pos[2] << endl;
  mesh.draw(shader, new_mv, THE_LIGHT, SPOT_LIGHT);
}
