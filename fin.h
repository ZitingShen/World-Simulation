#ifndef FIN_H
#define FIN_H

#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <cstring>
#include <string>
#include <float.h>
#include "common.h"
#include "read_ppm.h"
#include "light.h"
#include "glm/gtx/transform.hpp"

using namespace std;

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
    GLuint vao, vbo, ebo;
    GLuint textures[6];
    GLuint num_v;
    GLuint num_f; // must all be triangles
    vector<VERTEX> vertices;       // vertex pos and vertex normal
    vector<Image> texels;
    FACES     faces;
    glm::vec3 center;
    glm::vec3 size;
    GLuint texture_counter;
    /* Constructor */
    MESH();
    void setup(GLuint shader, glm::mat4& PROJ_MAT);
    void bind(GLuint shader);
    void compute_face_normal();
    void compute_vertex_normal();
    void draw(GLuint shader, glm::mat4& MV_MAT, glm::mat4& view_mat, LIGHT& THE_LIGHT, 
      spotlight SPOT_LIGHT);
    void draw(GLuint shader, glm::mat4& MV_MAT, glm::mat4& view_mat, LIGHT& THE_LIGHT, 
      spotlight SPOT_LIGHT,
      GLuint ebo);
    void rotate();
};

int read_mesh(string filename, MESH& mesh, GLuint shader, glm::mat4& PROJ_MAT);
void read_all_meshes(map<string, int>& filenames, vector<MESH>& all_meshes, GLuint shader,
  glm::mat4& PROJ_MAT);
void print_mesh_info(MESH& mesh);
GLuint make_bo(GLenum type, const void *buf, GLsizei buf_size);
#endif
