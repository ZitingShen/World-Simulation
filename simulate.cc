#include "simulate.h"

using namespace std;

int WIDTH, HEIGHT;
int IS_PAUSED = GLFW_TRUE;
int IS_ROTATED = GLFW_FALSE;
int PAUSE_TIME = 0;
int TOTAL_MESHES = 0;
double pmousex, pmousey;
glm::mat4 PROJ_MAT, MV_MAT;
LIGHT THE_LIGHT;
MESH BOIDS_MESH;
GLuint SHADER;
glm::vec3 CENTER, EYE, UP;

vector<BOID> BOIDS;
GOAL A_GOAL;
bool GUARDIAN = false;

int main(int argc, char *argv[]){
  if (!glfwInit ()) {
    std::cerr << "ERROR: could not start GLFW3" << std::endl;
    exit(EXIT_FAILURE);
  }
  
  glfwWindowHint (GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint (GLFW_CONTEXT_VERSION_MINOR, 2);
  glfwWindowHint (GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
  glfwWindowHint (GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  
  GLFWwindow* window = glfwCreateWindow(500, 500, "Make a World", 
  	NULL, NULL);
  if (!window){
    glfwTerminate();
    exit(EXIT_FAILURE);
  }
  glfwSetWindowPos(window, 100, 0);
  glfwMakeContextCurrent (window);

  glewExperimental = GL_TRUE;
  glewInit();
  
  SHADER = initshader("phong_vs.glsl", "phong_fs.glsl");
  init(window);

  glfwMakeContextCurrent(window);
  glfwSetWindowSizeCallback(window, reshape);
  glfwSetKeyCallback(window, keyboard);
  glfwSetFramebufferSizeCallback(window, framebuffer_resize);
  
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_CULL_FACE);
  glShadeModel(GL_SMOOTH);

  while(!glfwWindowShouldClose(window)) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glfwPollEvents();

    if(glfwGetWindowAttrib(window, GLFW_VISIBLE)){
      BOIDS_MESH->draw(SHADER, PROJ_MAT, MV_MAT, THE_LIGHT);
      glfwSwapBuffers(window);
    }

    if(!IS_PAUSED || PAUSE_TIME > 0) {
      change_view(PROJ_MODE, NONE);
      BOIDS->update();
      if (IS_PAUSED && PAUSE_TIME > 0) {
        print();
        PAUSE_TIME--;
      }
    }
  }
  glfwTerminate();
  exit(EXIT_SUCCESS);
}

void init(GLFWwindow* window) {
  glClearColor(0.0, 0.0, 0.0, 1.0);
  glColor3f(0.0, 0.0, 0.0);
  init_a_flock(BOIDS);
  init_goal(A_GOAL);

  //change_perspective(window);
  
  CENTER = glm::vec3(0.7*BLOCK, 0, 0.7*BLOCK);
  EYE = glm::vec3(EYE_X_DISPLACEMENT, EYE_Y_DISPLACEMENT, EYE_Z_DISPLACEMENT);
  UP = glm::vec3(0, 0, 1);
  MV_MAT = glm::lookAt(EYE, CENTER, UP);

  THE_LIGHT.light0 = glm::vec4(LIGHT_X, LIGHT_Y, LIGHT_Z, 0);
  for (auto itr_mesh = MESHES.begin(); itr_mesh != MESHES.end(); itr_mesh++)
    itr_mesh->compute_light_product(THE_LIGHT);
  glfwGetCursorPos(window, &pmousex, &pmousey);

  for (auto itr_mesh = MESHES.begin(); itr_mesh != MESHES.end(); itr_mesh++) {
    TOTAL_MESHES += itr_mesh->spin.size();
  }
}

void framebuffer_resize(GLFWwindow* window, int width, int height) {
   glViewport(0, 0, width, height);
}

void reshape(GLFWwindow* window, int w, int h) {
  MV_MAT = glm::lookAt(EYE, CENTER, UP);
}

void keyboard(GLFWwindow* window, int key, int scancode, int action, int mods) {
  if (action == GLFW_PRESS) {
    switch(key) {
   	  case GLFW_KEY_UP: {
      change_view(PROJ_MODE, ZOOM_IN);
      }
      break;

      case GLFW_KEY_DOWN: {
      change_view(PROJ_MODE, ZOOM_OUT);
      }
      break;

      case GLFW_KEY_Q:
      case GLFW_KEY_ESCAPE:
      glfwSetWindowShouldClose(window, GLFW_TRUE);
      break;
      default:
      break;
    }
  }
}

void print() {

}