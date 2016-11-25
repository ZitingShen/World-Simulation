#include "environment.h"

void init_environment_mesh(MESH& mesh, GLuint shader, glm::mat4& PROJ_MAT, int &TEXTURE_COUNTER) {
  mesh.texels.resize(6);
  if (!read_ppm(ENVIRONMENT_TEXTURE_POS_X, &mesh.texels[0])) {
    cerr << "ENVIRONMENT_MESH: FAILED TO LOAD TEXTURE POS X" << endl;
  }
  if (!read_ppm(ENVIRONMENT_TEXTURE_NEG_X, &mesh.texels[1])) {
    cerr << "ENVIRONMENT_MESH: FAILED TO LOAD TEXTURE NEG X" << endl;
  }
  if (!read_ppm(ENVIRONMENT_TEXTURE_POS_Y, &mesh.texels[2])) {
    cerr << "ENVIRONMENT_MESH: FAILED TO LOAD TEXTURE POS Y" << endl;
  }
  if (!read_ppm(ENVIRONMENT_TEXTURE_NEG_Y, &mesh.texels[3])) {
    cerr << "ENVIRONMENT_MESH: FAILED TO LOAD TEXTURE NEG Y" << endl;
  }
  if (!read_ppm(ENVIRONMENT_TEXTURE_POS_Z, &mesh.texels[4])) {
    cerr << "ENVIRONMENT_MESH: FAILED TO LOAD TEXTURE POS Z" << endl;
  }
  if (!read_ppm(ENVIRONMENT_TEXTURE_NEG_Z, &mesh.texels[5])) {
    cerr << "ENVIRONMENT_MESH: FAILED TO LOAD TEXTURE NEG Z" << endl;
  }
  mesh.texture_counter = TEXTURE_COUNTER;
  TEXTURE_COUNTER++;
  mesh.compute_face_normal();
  mesh.compute_vertex_normal();
  glGenVertexArrays(1, &mesh.vao);
  mesh.vbo = make_bo(GL_ARRAY_BUFFER, &mesh.vertices[0], mesh.vertices.size()*sizeof(VERTEX));
  mesh.ebo = make_bo(GL_ELEMENT_ARRAY_BUFFER, &mesh.faces.draw_indices[0],
    mesh.faces.draw_indices.size()*sizeof(GLuint));
  glBindVertexArray(mesh.vao);
  glBindBuffer(GL_ARRAY_BUFFER, mesh.vbo);
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

  glGenTextures(1, mesh.textures);
  glActiveTexture(GL_TEXTURE0+mesh.texture_counter);
  glBindTexture(GL_TEXTURE_CUBE_MAP, mesh.textures[0]+mesh.texture_counter);
  for (unsigned int i = 0; i < 6; i++) {
    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X+i, 0, GL_RGB, mesh.texels[i].sizeX,
      mesh.texels[i].sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, mesh.texels[i].data);
  }
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
  glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_MIN_FILTER,GL_LINEAR);

  glLinkProgram(shader);
  glUseProgram(shader);

  GLuint proj = glGetUniformLocation(shader, "Projection");
  glUniformMatrix4fv(proj, 1, GL_FALSE, glm::value_ptr(PROJ_MAT));
  GLuint shineness = glGetUniformLocation(shader, "Shineness");
  glUniform1f(shineness, SHINENESS);
}

void draw_environment(MESH& mesh, GLuint shader, glm::mat4& MV_MAT, LIGHT THE_LIGHT,
  glm::vec3& camera, spotlight SPOT_LIGHT, glm::vec3& environment_pos){
  THE_LIGHT.light0 = THE_LIGHT.light0*MV_MAT;
  glm::mat4 transformation = glm::translate(environment_pos);
  
  transformation = glm::scale(transformation, glm::vec3(ENVIRONMENT_SIZE/mesh.size[1], 
    ENVIRONMENT_SIZE/mesh.size[1], ENVIRONMENT_SIZE/mesh.size[1]));
  transformation = glm::translate(transformation, -mesh.center);
  
  GLuint tex = glGetUniformLocation(shader, "cube");
  glUniform1i(tex, mesh.texture_counter);
  
  GLuint model = glGetUniformLocation(shader, "Model");
  glUniformMatrix4fv(model, 1, GL_FALSE, glm::value_ptr(transformation));
  GLuint vw = glGetUniformLocation(shader, "View");
  glUniformMatrix4fv(vw, 1, GL_FALSE, glm::value_ptr(MV_MAT));
  GLuint light = glGetUniformLocation(shader, "LightPosition");
  glUniform4fv(light, 1, glm::value_ptr(THE_LIGHT.light0));
  GLuint ifNight = glGetUniformLocation(shader, "ifNight");
  glUniform1f(ifNight, THE_LIGHT.ifNight);
  GLuint cam = glGetUniformLocation(shader, "Camera");
  glUniform3fv(cam, 1, glm::value_ptr(camera));

  /* Adding spotlight */
  GLuint sl_pos = glGetUniformLocation(shader, "sp_position");
  glUniform4fv(sl_pos, 1, glm::value_ptr(SPOT_LIGHT.pos));

  GLuint sl_dir = glGetUniformLocation(shader, "coneDirection");
  glUniform3fv(sl_dir, 1, glm::value_ptr(SPOT_LIGHT.coneDirection));

  GLuint sl_angle = glGetUniformLocation(shader, "coneAngle");
  glUniform1f(sl_angle, SPOT_LIGHT.coneAngle);

  glBindVertexArray(mesh.vao);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.ebo);
  glDrawElements(GL_TRIANGLES, mesh.faces.draw_indices.size(), 
      GL_UNSIGNED_INT, (void*) 0);
}
