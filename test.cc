#include "island.h"

int main(){
	ofstream fout;
	string filename = "island.off";
	generate_island_mesh(fout, filename);
	return 0;
}