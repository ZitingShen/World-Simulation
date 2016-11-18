#include "island.h"

int main(){
	ofstream fout;
  vector<MESH> islands;
  unsigned int seed = 31415926;
	string filename = "island";
	string texture = "ppms/rock.ppm";
  generate_island_mesh(islands, seed, texture, fout, filename);
	return 0;
}
