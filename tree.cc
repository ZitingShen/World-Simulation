#include "tree.h"

void init_tree_mesh(MESH& mesh, MESH& island_mesh, const char *texture, GLuint shader, 
  glm::mat4& PROJ_MAT, vector<glm::vec3>& tree_pos, int TREE_NUM, int &TEXTURE_COUNTER) {
  for (unsigned int i = 0; i< mesh.num_v; i++) {
    mesh.vertices[i].pos -= mesh.center;
    if (mesh.vertices[i].pos[2] < 0) {
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
  float high_point = -FLT_MAX;
  int x = 0, y = 0;
  for(int i = 0; i <= SUBDIVISIONS; i++) {
    for(int j = 0; j <= SUBDIVISIONS; j++) {
      if (island_mesh.vertices[get_index(i, j)].pos[2] > high_point) {
        high_point = island_mesh.vertices[get_index(i, j)].pos[2];
        x = i;
        y = j;
      }
    }
  }
  for (int i = 0; i < TREE_NUM; i++) {
    int random1 = rand() % (SUBDIVISIONS/2) - (SUBDIVISIONS/4);
    int random2 = rand() % (SUBDIVISIONS/2) - (SUBDIVISIONS/4);
    tree_pos.push_back(island_mesh.vertices[get_index(x+random1, y+random2)].pos);
  }
  srand(time(NULL));
  mesh.texels.resize(1);
  if (!read_ppm(texture, &mesh.texels[0])) {
    cerr << "TREE: FAILED TO LOAD TEXTURE" << endl;
  }
  mesh.texture_counter = TEXTURE_COUNTER;
  TEXTURE_COUNTER++;
  mesh.compute_face_normal();
  mesh.compute_vertex_normal();
  mesh.setup(shader, PROJ_MAT);
}

void draw_tree(MESH& mesh, GLuint shader, glm::mat4& MV_MAT, LIGHT THE_LIGHT,
               spotlight SPOT_LIGHT, vector<glm::vec3>& tree_pos, float TREE_SIZE){
  
  for (unsigned int i = 0; i < tree_pos.size(); i++) {
    THE_LIGHT.light0 = THE_LIGHT.light0*MV_MAT;
    SPOT_LIGHT.pos = SPOT_LIGHT.pos*MV_MAT;
    
    glm::mat4 transformation = glm::translate(tree_pos[i]+glm::vec3(0, 0, TREE_SIZE*0.8f));
    transformation = glm::scale(transformation, glm::vec3(TREE_SIZE/mesh.size[2], TREE_SIZE/mesh.size[2], 
      TREE_SIZE/mesh.size[2]));
    mesh.draw(shader,MV_MAT, transformation, THE_LIGHT, SPOT_LIGHT);
  }
}