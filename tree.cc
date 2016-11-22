#include "tree.h"

void init_tree_mesh(MESH& mesh, MESH& island_mesh, const char *texture, GLuint shader, 
  glm::mat4& PROJ_MAT, vector<glm::vec3>& tree_pos, int TREE_NUM) {
  for (unsigned int i = 0; i< mesh.num_v; i++) {
    mesh.vertices[i].pos -= mesh.center;
    if (mesh.vertices[i].pos[2] < 0 && 
      (mesh.vertices[i].pos[1] < 0.2f && mesh.vertices[i].pos[1] > -0.2f)) {
      mesh.vertices[i].tex_coords = glm::vec2(
        static_cast<float>(rand())/static_cast<float>(RAND_MAX)*0.5f, 
        static_cast<float>(rand())/static_cast<float>(RAND_MAX)*0.5f);
    } else {
      mesh.vertices[i].tex_coords = glm::vec2(
        static_cast<float>(rand())/static_cast<float>(RAND_MAX)*0.5f+0.5f, 
        static_cast<float>(rand())/static_cast<float>(RAND_MAX)*0.5f+0.5f);
    }
  }
  srand(SEED);
  for (int i = 0; i < TREE_NUM; i++) {
    int random_pos = rand() / (int) WORLD_SIZE;
    tree_pos.push_back(island_mesh.vertices[random_pos].pos);
  }
  srand(time(NULL));
  mesh.texels.resize(1);
  if (!read_ppm(texture, &mesh.texels[0])) {
    cerr << "TREE: FAILED TO LOAD TEXTURE" << endl;
  }
  mesh.compute_face_normal();
  mesh.compute_vertex_normal();
  mesh.setup(shader, PROJ_MAT);
}

void draw_tree(MESH& mesh, GLuint shader, glm::mat4& MV_MAT, LIGHT THE_LIGHT,
               spotlight SPOT_LIGHT, vector<glm::vec3>& tree_pos, float TREE_SIZE){
  
  for (unsigned int i = 0; i < tree_pos.size(); i++) {
    glm::mat4 new_mv = MV_MAT;
    THE_LIGHT.light0 = THE_LIGHT.light0*MV_MAT;
    SPOT_LIGHT.pos = SPOT_LIGHT.pos*MV_MAT;
    
    new_mv = glm::translate(new_mv, tree_pos[i]+glm::vec3(0, 0, TREE_SIZE*0.8f));
    new_mv = glm::scale(new_mv, glm::vec3(TREE_SIZE/mesh.size[2], TREE_SIZE/mesh.size[2], 
      TREE_SIZE/mesh.size[2]));
    mesh.draw(shader, new_mv, THE_LIGHT, SPOT_LIGHT);
  }
}