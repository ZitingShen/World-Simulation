#include <vector>
#include "fin.h"// for mesh data structure
#include <fstream>
#include <iostream>
#include <string>
#include <time.h>
#include <glm/glm.hpp>
#include <math.h>
#include "assert.h"

#define WORLD_SIZE 1000
#define ISLAND_SIZE 10
#define SUBDIVISIONS 16   // has to be an even number
#define PRECIPICE 20
#define PERTUBE_LEVEL 20

using namespace std;

void get_num_v(int& num_v);
void get_num_f(int& num_f);
void get_all_vertices(int num_v, vector<glm::vec3>& all_vertices);
int get_index(int x, int y);
void create_precipice(vector<glm::vec3>& all_vertices);

float get_random();
float add_noise(int& noise);

void pertube(vector<glm::vec3>& all_vertices,
             int x_low,
             int y_low,
             int x_high,
             int y_high,
             float pertube_level);

void generate_faces(vector<int>& all_faces);

void generate_island_mesh(/*MESH& island,*/ofstream& fout, string filename);

