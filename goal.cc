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

void init_goal_mesh(MESH& mesh, GLuint shader) {
  mesh.num_v = 8;
  mesh.num_f = 12;
  mesh.vertices.resize(8);
  for (int i = 0; i< 8; i++) {
    mesh.vertices[i].pos = glm::vec3(CUBE_VERTICES[i][0], 
      CUBE_VERTICES[i][1], CUBE_VERTICES[i][2]);
    mesh.vertices[i].tex_coords = A_GOAL_TEX[i];
  }
  for (int i = 0; i < CUBE_INDICES; i++)
    mesh.faces.draw_indices.add(CUBE_INDICES[i]);
  if (!read_ppm("ppms/glass.ppm", &mesh.texels[0])) {
    cerr << "Fail loading texture: ppms/glass.ppm" << endl;
  }
  mesh.compute_face_normal();
  mesh.compute_vertex_normal();
  mesh.setup(shader);
}

void draw_a_flock(GOAL& goal, MESH& mesh, GLuint shader, 
  glm::mat4& PROJ_MAT, glm::mat4 MV_MAT, LIGHT& THE_LIGHT){
  MV_MAT = glm::translate(MV_MAT, a_flock[i].pos);
  mesh.draw(shader, PROJ_MAT, MV_MAT, THE_LIGHT);
}