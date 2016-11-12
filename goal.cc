#include "goal.h"

using namespace std;

GOAL::_goal(){
  pos = DEFAULT_GOAL_SPAWN_POSITION;
  velocity = DEFAULT_GOAL_SPAWN_VELOCITY;
  MOVE_ALONG_X_NEGATIVE = false;
  MOVE_ALONG_X_POSITIVE = false;
  MOVE_ALONG_Y_NEGATIVE = false;
  MOVE_ALONG_Y_POSITIVE = false;
  ACCELERATE = false;
  DECELERATE = false;
}

void update_goal_velocity(GOAL& goal){
  //float temp_z;
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

void init_goal_mesh(MESH& mesh, GLuint shader) {
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
  if (!read_ppm("ppms/fire.ppm", &mesh.texels[0])) {
    cerr << "Fail loading texture: ppms/fire.ppm" << endl;
  }
  mesh.compute_face_normal();
  mesh.compute_vertex_normal();
  mesh.setup(shader);
}

void draw_a_goal(GOAL& goal, MESH& mesh, GLuint shader, 
  glm::mat4& PROJ_MAT, glm::mat4 MV_MAT, LIGHT THE_LIGHT){
  THE_LIGHT.light0 = THE_LIGHT.light0*MV_MAT;
  MV_MAT = glm::translate(MV_MAT, goal.pos);
  mesh.draw(shader, PROJ_MAT, MV_MAT, THE_LIGHT);
}