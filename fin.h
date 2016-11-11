#ifndef FIN_H
#define FIN_H

#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <cstring>
#include "common.h"
#include "glm/gtx/transform.hpp"

using namespace std;


typedef struct _light{
 // using light
 glm::vec4 diffuse0 = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
 glm::vec4 ambient0 = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
 glm::vec4 specular0 = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
 glm::vec4 light0;
 glm::vec4 dropoff_coeff = glm::vec4(0.1f, 0.1f, 0.1f, 0.1f); //a, b, c, d
} LIGHT;

// TODO
struct TEXTURE{
  Image texture0;
  GLfloat shineness;
};

struct VERTEX{
  glm::vec3 pos;
  glm::vec3 normal;
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
    vector<TEXTURE> texture;
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
};

int read_mesh(string filename, MESH& mesh, int count, GLuint shader);
void read_all_meshes(map<string, int>& filenames, vector<MESH>& all_meshes, GLuint shader);
void print_mesh_info(MESH& mesh);
#endif
