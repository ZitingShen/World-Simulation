#include "island.h"

int SUBDIVISIONS = 128;
int ISLAND_SIZE = WORLD_SIZE / SUBDIVISIONS;

void set_island(int new_division){
  SUBDIVISIONS = new_division;
  ISLAND_SIZE = WORLD_SIZE / SUBDIVISIONS;
  cout << "new SUBDIVISIONS = " << SUBDIVISIONS << " new ISLAND_SIZE = " << ISLAND_SIZE << endl;
}

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
  int x_min = -(floor(side_v/2) * (ISLAND_SIZE + get_random() * ISLAND_SIZE));
  int y_min = -(floor(side_v/2) * (ISLAND_SIZE + get_random() * ISLAND_SIZE));
  for (int i=0; i<side_v; i++){
    for (int j=0; j<side_v; j++){
      all_vertices[count].x = x_min + j*ISLAND_SIZE;
      all_vertices[count].y = y_min + i*ISLAND_SIZE;
      all_vertices[count].z = 0;
      count++;
    }
  }
  all_vertices[get_index(0, SUBDIVISIONS/2)].z = PRECIPICE + get_random() * PERTUBE_LEVEL;
  all_vertices[get_index(SUBDIVISIONS/2, 0)].z = PRECIPICE + get_random() * PERTUBE_LEVEL;
  all_vertices[get_index(SUBDIVISIONS, SUBDIVISIONS/2)].z = PRECIPICE + get_random() * PERTUBE_LEVEL;
  all_vertices[get_index(SUBDIVISIONS/2, SUBDIVISIONS)].z = PRECIPICE + get_random() * PERTUBE_LEVEL;
  all_vertices[get_index(SUBDIVISIONS/2, SUBDIVISIONS/2)].z = 5 * PRECIPICE + get_random() * PERTUBE_LEVEL;
}

int get_index(int x, int y){
  return y*(SUBDIVISIONS+1) + x;
}

float get_random(){
  return static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
}

void create_precipice(vector<glm::vec3>& all_vertices){
  pertube(all_vertices, 0, 0, SUBDIVISIONS, SUBDIVISIONS, (float)PERTUBE_LEVEL);
}

bool on_edge(int index){
  if (index <= SUBDIVISIONS ||
      index % SUBDIVISIONS == 0 ||
      (index-1) % SUBDIVISIONS == 0 ||
      index >= (SUBDIVISIONS * (SUBDIVISIONS+1)))
   return true;
  else
   return false;
}

void get_island_centre(MESH& island, glm::vec3& centre){
  int index = island.vertices.size()/2;
  centre = glm::vec3(island.vertices[index].pos.x,
                     island.vertices[index].pos.y,
                     island.vertices[index].pos.z);
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
  //cout << "pertube level: " << pertube_level << endl;
  int x_mid = (x_low+x_high)/2;
  int y_mid = (y_low+y_high)/2;

  /* pertubing on five points */
  if ( (int)pertube_level !=  PERTUBE_LEVEL){
  //add_noise(noise);
  if (all_vertices[get_index(x_mid, y_low)].z == 0)
  all_vertices[get_index(x_mid, y_low)].z = 0.5 * (all_vertices[get_index(x_low, y_low)].z
                                                      + all_vertices[get_index(x_high, y_low)].z)
                                                      + get_random() * static_cast<float>(pertube_level);
  //add_noise(noise);
  if (all_vertices[get_index(x_mid, y_mid)].z == 0)
  all_vertices[get_index(x_low, y_mid)].z = 0.5 * (all_vertices[get_index(x_low, y_low)].z
                                                   + all_vertices[get_index(x_low, y_high)].z)
                                                   + get_random() * static_cast<float>(pertube_level);
  //add_noise(noise);
  if (all_vertices[get_index(x_mid, y_high)].z == 0)
  all_vertices[get_index(x_mid, y_high)].z = 0.5 * (all_vertices[get_index(x_low, y_high)].z
                                                   + all_vertices[get_index(x_high, y_high)].z)
                                                   + get_random() * static_cast<float>(pertube_level);
  //add_noise(noise);
  if (all_vertices[get_index(x_high, y_mid)].z == 0)
  all_vertices[get_index(x_high, y_mid)].z = 0.5 * (all_vertices[get_index(x_high, y_low)].z
                                                   + all_vertices[get_index(x_high, y_high)].z)
                                                   + get_random() * static_cast<float>(pertube_level);
  //add_noise(noise);
  if (all_vertices[get_index(x_mid, y_mid)].z == 0)
  all_vertices[get_index(x_mid, y_mid)].z = 0.25  * (all_vertices[get_index(x_low, y_mid)].z
                                                     + all_vertices[get_index(x_high, y_mid)].z
                                                     + all_vertices[get_index(x_low, y_mid)].z
                                                     + all_vertices[get_index(x_mid, y_high)].z)
                                                     + get_random() * static_cast<float>(pertube_level);
  }

  float new_pertube_level = 0.5* pertube_level;

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

void generate_faces(vector<GLuint>& all_faces){
  int index;
  /* add all faces on the surface */
  for(int i=0; i<SUBDIVISIONS; i++){
      for (int j=0; j<SUBDIVISIONS; j++){
      index = get_index(j, i);
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

void generate_island_mesh(vector<MESH>& island, GLuint shader, glm::mat4& PROJ_MAT) {
                          //ofstream& fout, string filename) {
  int num_v, num_f;
  string current_file;
  vector<vector<glm::vec3> > all_vertices;
  srand(SEED); // using given seed (UNSIGNED INT)

  island.resize(3); // using three levels of island mesh
  all_vertices.resize(3);
  for (int island_index=0; island_index<3; island_index++){
    //cout << "getting no. " << island_index << endl;
    set_island(SUBDIVISIONS*2);
    get_num_v(num_v);
    get_num_f(num_f);

    /* data generation */
    get_all_vertices(num_v, all_vertices[island_index]);
    create_precipice(all_vertices[island_index]);
    generate_faces(island[island_index].faces.draw_indices);

    /* File IO */
    /*
    current_file = filename + to_string(island_index) + ".off";
    //cout << current_file << endl;
    fout.open(current_file);
    if(!(fout.good() && fout.is_open())){
        cerr << "ISLAND_MESH: FAILED TO OPEN FILE" << endl;
        exit(1);
    }
    fout << "OFF" << endl;
    fout << num_v << " " << num_f << " " << num_e << endl;

    for (int i=0; i<num_v;i++){
      fout << all_vertices[island_index][i].x << " "
           << all_vertices[island_index][i].y << " "
           << all_vertices[island_index][i].z << " " << endl;
    }

    for (int i=0; i<num_f; i++){
      fout << 3 << " "
           << island[island_index].faces.draw_indices[i*3] << " "
           << island[island_index].faces.draw_indices[i*3 + 1] << " "
           << island[island_index].faces.draw_indices[i*3 + 2] << " " << endl;
    }

    fout.close();
    //cout << "fout done" << endl;
    */

    /* setting up MESH */
    
    island[island_index].num_v = num_v;
    island[island_index].num_f = num_f;
    island[island_index].vertices.resize(num_v); // resize to num_v
    for (int i=0; i<num_v; i++) {
      island[island_index].vertices[i].pos = all_vertices[island_index][i];
      island[island_index].vertices[i].tex_coords[0] = island[island_index].vertices[i].pos[0]/WORLD_SIZE-0.1f;
      island[island_index].vertices[i].tex_coords[1] = island[island_index].vertices[i].pos[1]/WORLD_SIZE-0.1f;
    }
    island[island_index].texels.resize(2);
    if (!read_ppm(ISLAND_TEXTURE, &island[island_index].texels[0])) {
      cerr << "ISLAND_MESH: FAILED TO LOAD TEXTURE" << endl;
    }
    if (!read_ppm(SNOW_TEXTURE, &island[island_index].texels[1])) {
      cerr << "ISLAND_MESH: FAILED TO LOAD TEXTURE" << endl;
    }

    island[island_index].compute_face_normal();
    island[island_index].compute_vertex_normal();
    island[island_index].setup(shader, PROJ_MAT);
  }
  srand(time(NULL)); // reset to time(NULL)
}

void draw_island(vector<MESH>& meshes, GLuint shader, glm::mat4& MV_MAT,
  LIGHT THE_LIGHT) {
  THE_LIGHT.light0 = THE_LIGHT.light0*MV_MAT;
  glm::mat4 new_mv = MV_MAT;
  //new_mv = glm::translate(new_mv, a_flock[i].pos);
  GLuint ifSnow = glGetUniformLocation(shader, "ifSnow");
  glUniform1i(ifSnow, 1);
  meshes[2].draw(shader, new_mv, THE_LIGHT);
}
