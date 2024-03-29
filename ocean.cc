#include "ocean.h"

void init_ocean_mesh(MESH& mesh, GLuint shader, glm::mat4& PROJ_MAT, 
  int &TEXTURE_COUNTER) {
  mesh.num_v = 4;
  mesh.num_f = 2;
  mesh.vertices.resize(4);
  for (int i = 0; i< 4; i++) {
    mesh.vertices[i].pos = glm::vec3(OCEAN_VERTICES[i][0], OCEAN_VERTICES[i][1], 
      OCEAN_VERTICES[i][2]);
    mesh.vertices[i].tex_coords = glm::vec2(OCEAN_TEX[i][0], OCEAN_TEX[i][1]);
  }
  for (int i = 0; i < 6; i++)
    mesh.faces.draw_indices.push_back(OCEAN_INDICES[i]);
  mesh.texels.resize(1);
  if (!read_ppm(OCEAN_TEXTURE, &mesh.texels[0])) {
    cerr << "OCEAN_MESH: FAILED TO LOAD TEXTURE" << endl;
  }
  mesh.texture_counter = TEXTURE_COUNTER;
  TEXTURE_COUNTER++;
  mesh.compute_face_normal();
  mesh.compute_vertex_normal();
  mesh.setup(shader, PROJ_MAT);
}

void draw_ocean(MESH& mesh, GLuint shader, glm::mat4& MV_MAT, LIGHT THE_LIGHT,
                spotlight SPOT_LIGHT){
  glm::mat4 transformation;
  THE_LIGHT.light0 = THE_LIGHT.light0*MV_MAT;
  SPOT_LIGHT.pos = SPOT_LIGHT.pos*MV_MAT;

  mesh.draw(shader, MV_MAT, transformation, THE_LIGHT, SPOT_LIGHT);
}
