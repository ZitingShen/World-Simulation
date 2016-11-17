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
#define ISLAND_SIZE 100
#define SUBDIVISIONS 8
#define PRECIPICE 500

using namespace std;

void get_num_v(int& num_v);
int get_num_v_helper(int levels);
void get_num_f(int num_v, int& num_f);
void get_all_vertices(int num_v, vector<glm::vec3>& all_vertices);
void create_precipice(vector<glm::vec3>& all_vertices);
void pertube(vector<glm::vec3>::iterator start,
             vector<glm::vec3>::iterator mid,
             vector<glm::vec3>::iterator end,
             float pertube_range);
void generate_island_mesh(MESH& island, ofstream& fout, string filename);

