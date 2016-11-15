#include "island.h"

void get_num_v(int& num_v){
  num_v = get_num_v_helper(SUBDIVISIONS);
  num_v = num_v*num_v;
}

int get_num_v_helper(int levels){
  if (levels == 0 || levels == 1)
	return 2;
  else
	return 2*get_num_v_helper(levels-1) - 1;
}

void get_num_f(int num_v, int& num_f){
  num_f = (num_v-1) * (num_v-1) * 2  // triangles on the surface
		   + (num_v-2) * 4 			   // triangles on the sides
		   + 2;						   // triangles at the bottoms
}

void get_all_vertices(int num_v, vector<glm::vec3>& all_vertices){
  all_vertices.resize(num_v);
  int side_v = sqrt(num_v);
  int count = 0;
  int x_min = -(floor(side_v/2) * ISLAND_SIZE);
  int y_min = -(floor(side_v/2) * ISLAND_SIZE);
  for (int i=0; i<side_v; i++){
    for (int j=0; j<side_v; j++){
      all_vertices[count].x = x_min + j*ISLAND_SIZE;
      all_vertices[count].y = y_min + i*ISLAND_SIZE;
      all_vertices[count].z = 0;
      count++;
    }
  }
}

void create_precipice(vector<glm::vec3>& all_vertices){

}

void pertube(vector<glm::vec3>::iterator start,
             vector<glm::vec3>::iterator end,
             float pertube_range){

}

void generate_island_mesh(MESH& island, ofstream& fout, string filename){
  int num_v, num_f, num_e;
  vector<glm::vec3> all_vertices;

  fout.open(filename);
  if(!(fout.good() && fout.is_open())){
  	cerr << "ISLAND_MESH: FAILED TO OPEN FILE\n" << endl;
  	exit(1);
  }

  get_num_v(num_v);
  get_num_f(num_v, num_f);
  num_e = 0;
  fout << "OFF" << endl;

  fout.close();
}
