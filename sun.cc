#include "sun.h"

void init_sun_mesh(MESH& mesh, GLuint shader, glm::mat4& PROJ_MAT, int &TEXTURE_COUNTER) {
  for (unsigned int i = 0; i< mesh.num_v; i++) {
    mesh.vertices[i].tex_coords = glm::vec2(
      static_cast<float>(rand())/static_cast<float>(RAND_MAX), 
      static_cast<float>(rand())/static_cast<float>(RAND_MAX));
  }
  mesh.texels.resize(1);
  if (!read_ppm(SUN_TEXTURE, &mesh.texels[0])) {
    cerr << "SUN_MESH: FAILED TO LOAD TEXTURE" << endl;
  }
  mesh.texture_counter = TEXTURE_COUNTER;
  TEXTURE_COUNTER++;
  mesh.compute_face_normal();
  mesh.compute_vertex_normal();
  mesh.setup(shader, PROJ_MAT);
}

void draw_a_sun(glm::vec3& sun_pos, glm::vec3& island_center, MESH& mesh, GLuint shader, 
  glm::mat4& MV_MAT, LIGHT THE_LIGHT, spotlight SPOT_LIGHT){
  THE_LIGHT.light0 = THE_LIGHT.light0*MV_MAT;
  SPOT_LIGHT.pos = SPOT_LIGHT.pos*MV_MAT;
  glm::mat4 view_mat = glm::translate(glm::vec3(sun_pos[0], SUN_RADIUS*glm::cos(sun_pos[1]), 
    SUN_RADIUS*glm::sin(sun_pos[2]))+island_center);
  view_mat = glm::scale(view_mat, glm::vec3(SUN_SIZE/mesh.size[0], SUN_SIZE/mesh.size[1], 
    SUN_SIZE/mesh.size[2]));
  view_mat = glm::translate(view_mat, -mesh.center);

  mesh.draw(shader, MV_MAT, view_mat, THE_LIGHT, SPOT_LIGHT);
}

void update_sun_pos(glm::vec3& sun_pos){
  sun_pos[1] += SUN_MOVEMENT;
  sun_pos[2] += SUN_MOVEMENT;
}
