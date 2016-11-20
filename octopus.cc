#include "octopus.h"

void init_octopus_mesh(MESH& mesh, GLuint shader, glm::mat4& PROJ_MAT) {
  for (unsigned int i = 0; i< mesh.num_v; i++) {
    mesh.vertices[i].tex_coords = glm::vec2(
      static_cast<float>(rand())/static_cast<float>(RAND_MAX), 
      static_cast<float>(rand())/static_cast<float>(RAND_MAX));
  }
  mesh.texels.resize(1);
  if (!read_ppm(ISLAND_TEXTURE, &mesh.texels[0])) {
    cerr << "OCTOPUS_MESH: FAILED TO LOAD TEXTURE" << endl;
  }
  mesh.compute_face_normal();
  mesh.compute_vertex_normal();
  mesh.setup(shader, PROJ_MAT);
}

void draw_octopus(MESH& mesh, GLuint shader, glm::mat4& MV_MAT, LIGHT THE_LIGHT) {
  glm::mat4 new_mv = MV_MAT;
  THE_LIGHT.light0 = THE_LIGHT.light0*MV_MAT;
  new_mv = glm::translate(new_mv, OCTOPUS_POS-mesh.center);
  new_mv = glm::scale(new_mv, glm::vec3(OCTOPUS_SIZE/mesh.size[0], OCTOPUS_SIZE/mesh.size[1], 
    OCTOPUS_SIZE/mesh.size[2]));
  new_mv = glm::rotate(new_mv, 200*DEGREE_TO_RADIAN, glm::vec3(1.0f, 0.0f, 0.0f));
  mesh.draw(shader, new_mv, THE_LIGHT);
}