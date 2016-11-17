#include "sun.h"

void init_sun_mesh(MESH& mesh, GLuint shader, glm::mat4& PROJ_MAT) {
  mesh.num_v = SUN_POINTS;
  mesh.num_f = SUN_POINTS;
  mesh.vertices.resize(SUN_POINTS);
  for (int i = 0; i< SUN_POINTS; i++) {
    glm::vec3 new_point = glm::vec3(
    	static_cast<float>(rand())/static_cast<float>(RAND_MAX)*2-1,
    	static_cast<float>(rand())/static_cast<float>(RAND_MAX)*2-1, 
      static_cast<float>(rand())/static_cast<float>(RAND_MAX)*2-1);
    mesh.vertices[i].pos = SUN_SIZE*glm::normalize(new_point);
    mesh.vertices[i].tex_coords = glm::vec2(
    	static_cast<float>(rand())/static_cast<float>(RAND_MAX), 
      static_cast<float>(rand())/static_cast<float>(RAND_MAX));
  }
  for (int i = 0; i < SUN_POINTS; i++)
    mesh.faces.draw_indices.push_back(i);
  mesh.texels.resize(1);
  if (!read_ppm("ppms/fire.ppm", &mesh.texels[0])) {
    cerr << "Fail loading texture: ppms/fire.ppm" << endl;
  }
  mesh.compute_face_normal();
  mesh.compute_vertex_normal();
  mesh.setup(shader, PROJ_MAT);
}

void draw_a_sun(glm::vec3& sun_pos, MESH& mesh, GLuint shader, glm::mat4& MV_MAT, LIGHT THE_LIGHT) {
  glm::mat4 new_mv = MV_MAT;
  THE_LIGHT.light0 = THE_LIGHT.light0*MV_MAT;
  new_mv = glm::translate(new_mv, glm::vec3(sun_pos[0], SUN_RADIUS*glm::cos(sun_pos[1]), 
    SUN_RADIUS*glm::sin(sun_pos[2])));
  mesh.draw(shader, new_mv, THE_LIGHT);
}

void update_sun_pos(glm::vec3& sun_pos) {
  sun_pos[1] += SUN_MOVEMENT;
  sun_pos[2] += SUN_MOVEMENT;
}