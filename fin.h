#ifndef FIN_H
#define FIN_H

#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <cstring>
#include "common.h"
#include "read_ppm.h"
#include "glm/gtx/transform.hpp"

using namespace std;


typedef struct _light{
 // using light
 glm::vec4 light0;
 glm::vec4 dropoff_coeff = glm::vec4(0.1f, 0.1f, 0.1f, 0.1f);
} LIGHT;

struct VERTEX{
  glm::vec3 pos;
  glm::vec3 normal;
  glm::vec2 tex_coords;
};

struct FACES{
  vector<GLuint> draw_indices; // triangulised indices
  vector<glm::vec3> normal;     // nromals of triangulaised faces
};

class MESH {
  public:
    /* public data member */
    GLuint num_v;
    GLuint num_f; // must all be triangles
    vector<VERTEX> vertices;       // vertex pos and vertex normal
    vector<Image> texels;
    FACES     faces;
    /* Constructor */
    MESH();
    void setup(GLuint shader);
    void bind(GLuint shader);
    void compute_face_normal();
    void compute_vertex_normal();
    void draw(GLuint shader, glm::mat4& PROJ_MAT, glm::mat4& MV_MAT, 
      LIGHT& THE_LIGHT);
    void rotate();
  private:
    GLuint vao, vbo, ebo;
    GLuint textures[6];
};

int read_mesh(string filename, MESH& mesh, int count, GLuint shader);
void read_all_meshes(map<string, int>& filenames, vector<MESH>& all_meshes, GLuint shader);
void print_mesh_info(MESH& mesh);
#endif
