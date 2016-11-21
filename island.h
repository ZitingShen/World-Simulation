#ifndef ISLAND_H
#define ISLAND_H

#include <vector>
#include "fin.h"// for mesh data structure
#include <iostream>
#include <string>
#include <time.h>
#include <glm/glm.hpp>
#include "common.h"
#include "assert.h"
#include "read_ppm.h"

const glm::vec3 ISLAND_POS = glm::vec3(0.0f, 0.0f, 0.0f);

extern int SUBDIVISIONS;   // has to be an even number
extern int ISLAND_SIZE;

using namespace std;

void set_island(int new_division);

void get_num_v(int& num_v);
void get_num_f(int& num_f);
void get_all_vertices(int num_v, vector<glm::vec3>& all_vertices);
int get_index(int x, int y);
void create_precipice(vector<glm::vec3>& all_vertices);
float get_random();

void pertube(vector<glm::vec3>& all_vertices,
             int x_low,
             int y_low,
             int x_high,
             int y_high,
             float pertube_level);

bool on_edge(int index);

void adjust_position(MESH& island);

void get_island_centre(MESH& island, glm::vec3& centre);

void generate_faces(vector<GLuint>& all_faces);

void generate_island_mesh(vector<MESH>& island, GLuint shader, glm::mat4& PROJ_MAT);
                          //ofstream& fout, string filename);
void draw_island(vector<MESH>& meshes, GLuint shader,
                 glm::mat4& MV_MAT, LIGHT THE_LIGHT, spotlight SPOT_LIGHT, glm::vec3& eye);
#endif
