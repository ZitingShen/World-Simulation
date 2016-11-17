#include "island.h"

void get_num_v(int& num_v){
  num_v = (SUBDIVISIONS+1) * (SUBDIVISIONS+1);
}
void get_num_f(int& num_f){
  num_f = SUBDIVISIONS*SUBDIVISIONS*2;
}

void get_all_vertices(int num_v, vector<glm::vec3>& all_vertices){

  if (SUBDIVISIONS % 2 != 0){
    cerr << "SUBDIVISIONS: HAS TO BE AN EVEN NUMBER." << endl;
    exit(3);
  }

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
  all_vertices[get_index(0, SUBDIVISIONS/2)].z = PRECIPICE;
  all_vertices[get_index(SUBDIVISIONS/2, 0)].z = PRECIPICE;
  all_vertices[get_index(SUBDIVISIONS, SUBDIVISIONS/2)].z = PRECIPICE;
  all_vertices[get_index(SUBDIVISIONS/2, SUBDIVISIONS)].z = PRECIPICE;
  all_vertices[get_index(SUBDIVISIONS/2, SUBDIVISIONS/2)].z = 2 * PRECIPICE;
}

int get_index(int x, int y){
  return y*(SUBDIVISIONS+1) + x;
}

float get_random(){
  return static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
}

float add_noise(int& noise){
  float yardstick = get_random();
  if (yardstick < 0.33)
    noise = -1;
  else if (yardstick)
}

void create_precipice(vector<glm::vec3>& all_vertices){
  pertube(all_vertices, 0, 0, SUBDIVISIONS, SUBDIVISIONS, (float)PERTUBE_LEVEL);
}

void pertube(vector<glm::vec3>& all_vertices,
             int x_low,
             int y_low,
             int x_high,
             int y_high,
             float pertube_level){
  if ((x_high - x_low == 1) && (y_high - y_low) == 1){
    return; // base case, pertubing finsihed
  }
  cout << "pertube level: " << pertube_level << endl;
  srand(time(NULL));
  int noise = 0;
  int x_mid = (x_low+x_high)/2;
  int y_mid = (y_low+y_high)/2;

  /* pertubing on five points */
  all_vertices[get_index(x_mid, y_low)].z += 0.25 * (all_vertices[get_index(x_low, y_low)].z
                                                     + all_vertices[get_index(x_high, y_low)].z)
                                                  +  get_random() * static_cast<float>(pertube_level);

  all_vertices[get_index(x_low, y_mid)].z += 0.25 * (all_vertices[get_index(x_low, y_low)].z
                                                   + all_vertices[get_index(x_low, y_high)].z)
                                                   +  get_random() * static_cast<float>(pertube_level);

  all_vertices[get_index(x_mid, y_high)].z += 0.25 * (all_vertices[get_index(x_low, y_high)].z
                                                   + all_vertices[get_index(x_high, y_high)].z)
                                                  +  get_random() * static_cast<float>(pertube_level);

  all_vertices[get_index(x_high, y_mid)].z += 0.25 * (all_vertices[get_index(x_high, y_low)].z
                                                   + all_vertices[get_index(x_high, y_high)].z)
                                                 +  get_random() * static_cast<float>(pertube_level);

  all_vertices[get_index(x_mid, y_mid)].z += 0.125  * (all_vertices[get_index(x_low, y_mid)].z
                                                    + all_vertices[get_index(x_high, y_mid)].z
                                                    + all_vertices[get_index(x_low, y_mid)].z
                                                    + all_vertices[get_index(x_mid, y_high)].z)
                                                    +  get_random() * static_cast<float>(pertube_level) * 2;

  float new_pertube_level = 0.5*pertube_level;

  pertube(all_vertices, x_low, y_low,
                        x_mid, y_mid, 
                        new_pertube_level);   //lower-left
  pertube(all_vertices, x_mid, y_low,
                        x_high, y_mid, 
                        new_pertube_level);  //lower-right
  pertube(all_vertices, x_low, y_mid,
                        x_mid, y_high, 
                        new_pertube_level);  //upper-left
  pertube(all_vertices, x_mid, y_mid,
                        x_high, y_high,
                        new_pertube_level); //upper-right
}

void generate_faces(vector<int>& all_faces){
  int index;
  /* add all faces on the surface */
  for(int i=0; i<SUBDIVISIONS; i++){
      for (int j=0; j<SUBDIVISIONS; j++){
      index = get_index(j, i);
      cout << index << endl;
      /* Lower Triangle */
      all_faces.push_back(index);
      all_faces.push_back(index+1);
      all_faces.push_back(index+1+SUBDIVISIONS);
      /* Higher Triangle */
      all_faces.push_back(index+1);
      all_faces.push_back(index+1+SUBDIVISIONS+1);
      all_faces.push_back(index+1+SUBDIVISIONS);
    }
  }
}
void generate_island_mesh(/*MESH& island,*/ofstream& fout, string filename){
  int num_v, num_f, num_e;
  vector<glm::vec3> all_vertices;
  vector<int> all_faces;

  fout.open(filename);
  if(!(fout.good() && fout.is_open())){
  	cerr << "ISLAND_MESH: FAILED TO OPEN FILE\n" << endl;
  	exit(1);
  }

  get_num_v(num_v);
  get_num_f(num_f);
  num_e = 0;
  fout << "OFF" << endl;
  fout << num_v << " " << num_f << " " << num_e << endl;

  get_all_vertices(num_v, all_vertices);
  create_precipice(all_vertices);
  generate_faces(all_faces);

  for (int i=0; i<(int)all_vertices.size();i++){
    fout << all_vertices[i].x << " "
         << all_vertices[i].y << " "
         << all_vertices[i].z << " " << endl;
  }

  for (int i=0; i<((int)all_faces.size()/3); i++){
    fout << 3 << " "
         << all_faces[i*3] << " "
         << all_faces[i*3 + 1] << " "
         << all_faces[i*3 + 2] << " " << endl;
  }

  fout.close();
}
