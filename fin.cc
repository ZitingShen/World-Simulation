#include "fin.h"

static GLuint make_bo(GLenum type, const void *buf, GLsizei buf_size) {
  GLuint bufnum;
  glGenBuffers(1, &bufnum);
  glBindBuffer(type, bufnum);
  glBufferData(type, buf_size, buf, GL_STATIC_DRAW);
  return bufnum;
}

MESH::MESH(){
  this->num_v = 0;
  this->num_f = 0;
}

void MESH::setup(GLuint shader, glm::mat4& PROJ_MAT){
  glGenVertexArrays(1, &this->vao);
  vbo = make_bo(GL_ARRAY_BUFFER, &vertices[0],
    vertices.size()*sizeof(VERTEX));
  ebo = make_bo(GL_ELEMENT_ARRAY_BUFFER, &faces.draw_indices[0],
    faces.draw_indices.size()*sizeof(GLuint));
  glBindVertexArray(vao);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBindAttribLocation(shader, POS_LOCATION, "vPosition");
  glEnableVertexAttribArray(POS_LOCATION);
  glVertexAttribPointer(POS_LOCATION, 3, GL_FLOAT,
                        GL_FALSE,
                        sizeof(VERTEX),
                        (GLvoid*)offsetof(VERTEX, pos));

  // Vertex Normals
  glBindAttribLocation(shader, NORMAL_LOCATION, "vNormal");
  glEnableVertexAttribArray(NORMAL_LOCATION);
  glVertexAttribPointer(NORMAL_LOCATION, 3, GL_FLOAT,
                        GL_FALSE,
                        sizeof(VERTEX),
                        (GLvoid*)offsetof(VERTEX, normal));
 
  glBindAttribLocation(shader, TEXTURE_LOCATION, "vTex");
  glEnableVertexAttribArray(TEXTURE_LOCATION);
  glVertexAttribPointer(TEXTURE_LOCATION, 2, GL_FLOAT,
                        GL_FALSE,
                        sizeof(VERTEX),
                        (GLvoid*)offsetof(VERTEX, tex_coords));

  glGenTextures(texels.size(), textures);
  for (unsigned int i = 0; i < texels.size(); i++) {
    glBindTexture(GL_TEXTURE_2D, textures[i]);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texels[i].sizeX,
      texels[i].sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, texels[i].data);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
  }

  glLinkProgram(shader);
  glUseProgram(shader);

  GLuint proj = glGetUniformLocation(shader, "Projection");
  glUniformMatrix4fv(proj, 1, GL_FALSE, glm::value_ptr(PROJ_MAT));
  GLuint shineness = glGetUniformLocation(shader, "Shineness");
  glUniform1f(shineness, SHINENESS);
}

void MESH::compute_face_normal(){
  glm::vec3 normal;
  // Assuming all vertices are given in counter-clock order
  for (unsigned int count=0; count<faces.draw_indices.size(); count+=3){
    normal = glm::cross(this->vertices[ this->faces.draw_indices[count + 2] ].pos
                      - this->vertices[ this->faces.draw_indices[count + 1] ].pos,
                        this->vertices[ this->faces.draw_indices[count    ] ].pos
                      - this->vertices[ this->faces.draw_indices[count + 1] ].pos );
    this->faces.normal.push_back(glm::normalize(normal));
  }
}

void MESH::compute_vertex_normal(){
  vector<vector<int> > faces_per_vertex(this->num_v, vector<int>());
  glm::vec3 normal;
  int num_triangles = this->faces.draw_indices.size() / 3;
  int count = 0;
  for (int i=0; i<num_triangles; i++){  // which face we are looking at
    for (int j=0; j<3; j++){  // all vertices in this face
      //cout << "count = " << count << " i= " << i << endl;
      faces_per_vertex[this->faces.draw_indices[count]].push_back(i);
      count++;
    }
  }
  //for (int i=0; i<(int)this->num_v; i++)
  //for (auto k = faces_per_vertex[i].begin(); k != faces_per_vertex[i].end(); k++)
  //  cout << (*k) << ' ';
  //  cout << endl;
  count = 0;
  for (int i=0; i<(int)this->num_v; i++){
    normal = glm::vec3(0, 0, 0);
    for (int j=0; j<(int)faces_per_vertex[i].size(); j++){
      normal += this->faces.normal[faces_per_vertex[i][j]];
    }
    normal = glm::normalize(normal * (1.0f / (float)faces_per_vertex[i].size()));
    this->vertices[i].normal[0] = normal[0];
    this->vertices[i].normal[1] = normal[1];
    this->vertices[i].normal[2] = normal[2];
    /*
    cout << this->vertices[i].normal[0] << " "
         << this->vertices[i].normal[1] << " "
         << this->vertices[i].normal[2] << " " << endl;
         */
  }
}

int read_mesh(string filename, MESH& mesh, GLuint shader, glm::mat4& PROJ_MAT){
  glm::vec3 local_max = glm::vec3(-FLT_MAX, -FLT_MAX, -FLT_MAX);
  glm::vec3 local_min = glm::vec3(FLT_MAX, FLT_MAX, FLT_MAX);
  vector<GLuint> holder_indices;
  string off;
  int vec_in_fac;
  int temp;
  ifstream my_fin;
  my_fin.open(filename);
  if (!my_fin.is_open()){
    cerr << "READ_MESH: CAN NOT OPEN FILE: " << filename <<endl;
    return GLFW_FALSE;
  }
  getline(my_fin, off);
  if (off.compare("OFF") != 0){
    cerr << "READ_MESH: NOT AN OFF FILE: " << filename <<endl;
    return GLFW_FALSE;
  }
  /* reading attributs */
  my_fin >> mesh.num_v;
  my_fin >> mesh.num_f;
  int num_e;
  my_fin >> num_e;
  /* reading vertices */
  mesh.vertices.resize(mesh.num_v);
  for (int i=0; i<(int)mesh.num_v; i++){
    for (int j=0; j<3; j++){
      my_fin >> mesh.vertices[i].pos[j];
    }
    local_max[0] = local_max[0]<mesh.vertices[i].pos[0]?mesh.vertices[i].pos[0]:local_max[0];
    local_max[1] = local_max[1]<mesh.vertices[i].pos[1]?mesh.vertices[i].pos[1]:local_max[1];
    local_max[2] = local_max[2]<mesh.vertices[i].pos[2]?mesh.vertices[i].pos[2]:local_max[2];
    local_min[0] = local_min[0]>mesh.vertices[i].pos[0]?mesh.vertices[i].pos[0]:local_min[0];
    local_min[1] = local_min[1]>mesh.vertices[i].pos[1]?mesh.vertices[i].pos[1]:local_min[1];
    local_min[2] = local_min[2]>mesh.vertices[i].pos[2]?mesh.vertices[i].pos[2]:local_min[2];
  }
  /* reading faces */
  for (int i=0; i<(int)mesh.num_f; i++){
    my_fin >> vec_in_fac;
    //mesh.faces.num_v.push_back(static_cast<GLuint>(vec_in_fac));
    if (vec_in_fac == 3){
      for (int j=0; j<vec_in_fac;j++){
        my_fin >> temp;
        mesh.faces.draw_indices.push_back(temp);
      }
    }else{ // when more than three vertices in a face
      for (int j=0; j<vec_in_fac;j++){
        my_fin >> temp;
        holder_indices.push_back(temp);
      }
      for (int j=0; j<(vec_in_fac-2);j++){
        mesh.faces.draw_indices.push_back(holder_indices[0]);
        mesh.faces.draw_indices.push_back(holder_indices[j+1]);
        mesh.faces.draw_indices.push_back(holder_indices[j+2]);
      }
    }
  }
  my_fin.close();
  mesh.center = (local_max+local_min)*0.5f;
  mesh.size = local_max-local_min;
  return GLFW_TRUE;
}

void read_all_meshes(vector<string>& filenames, vector<MESH>& all_meshes, GLuint shader,
  glm::mat4& PROJ_MAT){
  int i = 0;
  for (auto itr_file = filenames.begin(); itr_file != filenames.end(); itr_file++){
    //cout << "reading " << filenames[i] << endl;
    if(!read_mesh(*itr_file, all_meshes[i], shader, PROJ_MAT)){
      all_meshes.erase(all_meshes.begin()+i);
    } else {
      i++;
    }
  }
}

void print_mesh_info(MESH& mesh){
  cout << mesh.num_v << " ";
  cout << mesh.num_f << " " << endl;
  /* print vertcies */
  for (int i = 0; i<(int)mesh.num_v; i++){
    for (int j=0; j<3; j++){
      cout << mesh.vertices[i].pos[j] << " ";
    }
    cout << endl;
  }
  /* print draw faces */
  cout << (int)mesh.faces.draw_indices.size() / 3<< endl;
  for (int i=0; i <(int)mesh.faces.draw_indices.size(); i++){
    cout << (int)mesh.faces.draw_indices[i] << " ";
    if ((i+1) % 3 == 0)
      cout << endl;
  }
  /* print face normals */
  cout << (int)mesh.faces.normal.size()<< endl;
  for (int i=0; i <(int)mesh.faces.normal.size(); i++){
    cout << mesh.faces.normal[i][0] << " ";
    cout << mesh.faces.normal[i][1] << " ";
    cout << mesh.faces.normal[i][2] << " "  << endl;
  }
  /* print vertex normals */
  cout << (int)mesh.vertices.size()<< endl;
  for (int i=0; i <(int)mesh.vertices.size(); i++){
    cout << mesh.vertices[i].normal[0] << " ";
    cout << mesh.vertices[i].normal[2] << " ";
    cout << mesh.vertices[i].normal[3] << " "  << endl;
  }
}

void MESH::draw(GLuint shader, glm::mat4& MV_MAT, LIGHT& THE_LIGHT) {
  for (unsigned int i = 0; i < texels.size(); i++) {
    GLuint tex = glGetUniformLocation(shader, "tex"+i);
    glActiveTexture(GL_TEXTURE0+i);
    glBindTexture(GL_TEXTURE_2D, textures[i]);
    glUniform1i(tex, i);
  }
  
  GLuint mv = glGetUniformLocation(shader, "ModelView");
  glUniformMatrix4fv(mv, 1, GL_FALSE, glm::value_ptr(MV_MAT));
  GLuint light = glGetUniformLocation(shader, "LightPosition");
  glUniform4fv(light, 1, glm::value_ptr(THE_LIGHT.light0));
  GLuint ifNight = glGetUniformLocation(shader, "ifNight");
  glUniform1f(ifNight, THE_LIGHT.ifNight);

  glBindVertexArray(this->vao);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->ebo);
  glDrawElements(GL_TRIANGLES, this->faces.draw_indices.size(), 
      GL_UNSIGNED_INT, (void*) 0);
}
