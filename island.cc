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
  all_vertices[get_index(0, SUBDIVISIONS/2)].z = get_random() * PERTUBE_LEVEL;
  all_vertices[get_index(SUBDIVISIONS/2, 0)].z = get_random() * PERTUBE_LEVEL;
  all_vertices[get_index(SUBDIVISIONS, SUBDIVISIONS/2)].z = get_random() * PERTUBE_LEVEL;
  all_vertices[get_index(SUBDIVISIONS/2, SUBDIVISIONS)].z = get_random() * PERTUBE_LEVEL;
  all_vertices[get_index(SUBDIVISIONS/2, SUBDIVISIONS/2)].z = get_random() * PERTUBE_LEVEL+get_random()*PERTUBE_LEVEL+get_random()*PERTUBE_LEVEL;
}

int get_index(int x, int y){
  return y*(SUBDIVISIONS+1) + x;
}

float get_random(){
  return static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
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
  centre = island.vertices[island.vertices.size()/2].pos;
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

void generate_faces(vector<GLuint>& all_faces, int sub){
  int index;
  /* add all faces on the surface */
  for(int i=0; i<SUBDIVISIONS; i+=sub){
      for (int j=0; j<SUBDIVISIONS; j+=sub){
      index = get_index(j, i);
      /* Lower Triangle */
      all_faces.push_back(index);
      all_faces.push_back(index+sub);
      all_faces.push_back(index+sub+sub*SUBDIVISIONS);
      /* Higher Triangle */
      all_faces.push_back(index+sub);
      all_faces.push_back(index+sub+sub*SUBDIVISIONS+sub);
      all_faces.push_back(index+sub+sub*SUBDIVISIONS);
    }
  }
}

void generate_island_mesh(MESH& mesh, vector<vector<GLuint>>& index_groups, vector<GLuint>& ebos, 
  GLuint shader, glm::mat4& PROJ_MAT, int &TEXTURE_COUNTER) {
                          //ofstream& fout, string filename) {
  int num_v, num_f;
  string current_file;
  vector<glm::vec3> all_vertices;

  int subdivisions = SUBDIVISIONS_TIMES*SUBDIVISIONS_TIMES;
  index_groups.resize(3);
  ebos.resize(3);
  for (int i = 0; i < 3; i++){
    generate_faces(index_groups[i], subdivisions);
    ebos[i] = make_bo(GL_ELEMENT_ARRAY_BUFFER, &index_groups[i][0], 
      index_groups[i].size()*sizeof(GLuint));
    subdivisions /= SUBDIVISIONS_TIMES;
  }

  srand(SEED); // using given seed (UNSIGNED INT)
  get_num_v(num_v);
  get_num_f(num_f);

  /* data generation */
  mesh.num_v = num_v;
  mesh.num_f = num_f;
  get_all_vertices(num_v, all_vertices);
  pertube(all_vertices, 0, 0, SUBDIVISIONS, SUBDIVISIONS, (float)PERTUBE_LEVEL);
  srand(time(NULL)); // reset to time(NULL)
  /* setting up MESH */
  mesh.vertices.resize(num_v); // resize to num_v
  float sea_level = -FLT_MAX;
  for(int i = 0; i <= SUBDIVISIONS; i++) {
    sea_level = all_vertices[get_index(i, 0)].z>sea_level?all_vertices[get_index(i, 0)].z:sea_level;
    sea_level = all_vertices[get_index(0, i)].z>sea_level?all_vertices[get_index(0, i)].z:sea_level;
    sea_level = all_vertices[get_index(i, SUBDIVISIONS)].z>sea_level?all_vertices[get_index(i, SUBDIVISIONS)].z:sea_level;
    sea_level = all_vertices[get_index(SUBDIVISIONS, i)].z>sea_level?all_vertices[get_index(SUBDIVISIONS, i)].z:sea_level;
  }
  for (int i=0; i<num_v; i++) {
    mesh.vertices[i].pos = glm::vec3(all_vertices[i].x, all_vertices[i].y, 
      all_vertices[i].z - sea_level); // sunken island
    mesh.vertices[i].tex_coords[0] = mesh.vertices[i].pos[0]/WORLD_SIZE+0.85f;
    mesh.vertices[i].tex_coords[1] = mesh.vertices[i].pos[1]/WORLD_SIZE+0.85f;
  }
  mesh.texels.resize(2);
  if (!read_ppm(ISLAND_TEXTURE, &mesh.texels[0])) {
    cerr << "ISLAND_MESH: FAILED TO LOAD TEXTURE" << endl;
  }
  if (!read_ppm(SNOW_TEXTURE, &mesh.texels[1])) {
    cerr << "ISLAND_MESH: FAILED TO LOAD TEXTURE" << endl;
  }
  mesh.texture_counter = TEXTURE_COUNTER;
  TEXTURE_COUNTER += 2;

  mesh.faces.draw_indices = index_groups[2];
  mesh.compute_face_normal();
  mesh.compute_vertex_normal();
  mesh.setup(shader, PROJ_MAT);
}

void draw_island(MESH& mesh, vector<GLuint>& ebos, GLuint shader, 
  glm::mat4& MV_MAT, LIGHT THE_LIGHT, spotlight SPOT_LIGHT, glm::vec3& eye){

  THE_LIGHT.light0 = THE_LIGHT.light0*MV_MAT;
  SPOT_LIGHT.pos = SPOT_LIGHT.pos * MV_MAT;

  glm::mat4 new_mv = MV_MAT;
  new_mv = glm::translate(new_mv, ISLAND_POS);
  GLuint ifSnow = glGetUniformLocation(shader, "ifSnow");
  glUniform1i(ifSnow, 1);
  float distance = glm::distance(eye, ISLAND_POS);

  if(distance > 6000) {
    mesh.draw(shader, new_mv, THE_LIGHT, SPOT_LIGHT, ebos[0]);
  } else if (distance > 4000) {
    mesh.draw(shader, new_mv, THE_LIGHT, SPOT_LIGHT, ebos[1]);
  } else{
    mesh.draw(shader, new_mv, THE_LIGHT, SPOT_LIGHT, ebos[2]);
  }
  glUniform1i(ifSnow, 0);
}
