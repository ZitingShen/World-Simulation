#include "simulate.h"

using namespace std;

int ENABLE_FLOCK = GLFW_TRUE;
int ENABLE_GOAL = GLFW_TRUE;
int ENABLE_MOUNTAIN = GLFW_TRUE;
int WIDTH, HEIGHT;
int IS_PAUSED = GLFW_FALSE;
int PAUSE_TIME = 0;

glm::mat4 PROJ_MAT, MV_MAT = glm::mat4();
LIGHT THE_LIGHT;
MESH BOIDS_MESH, GOAL_MESH;
GLuint SHADER;

vector<BOID> A_FLOCK;
GOAL A_GOAL;
viewMode VIEW_MODE = DEFAULT;

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
    change_view(MV_MAT, VIEW_MODE, A_FLOCK, A_GOAL);

    if(!IS_PAUSED || PAUSE_TIME > 0) {
      update_goal_velocity(A_GOAL);
      update_goal_pos(A_GOAL);
      update_velocity(A_FLOCK, A_GOAL);
      apply_goal_attraction(A_FLOCK, A_GOAL);
      update_pos(A_FLOCK);
      if (IS_PAUSED && PAUSE_TIME > 0) {
        print_goal(A_GOAL);
        print_flock(A_FLOCK);
        PAUSE_TIME--;
      }
    }

    if(glfwGetWindowAttrib(window, GLFW_VISIBLE)){
      if (ENABLE_FLOCK)
        draw_a_flock(A_FLOCK, BOIDS_MESH, SHADER, PROJ_MAT, MV_MAT, 
          THE_LIGHT);
      if (ENABLE_GOAL)
        draw_a_goal(A_GOAL, GOAL_MESH, SHADER, PROJ_MAT, MV_MAT,
          THE_LIGHT);
      glfwSwapBuffers(window);
    }
  }
  glfwTerminate();
  exit(EXIT_SUCCESS);
}

void init(GLFWwindow* window) {
  glClearColor(0.0, 0.0, 0.0, 1.0);
  glColor3f(0.0, 0.0, 0.0);
  srand(time(NULL));
  SHADER = initshader("phong_vs.glsl", "phong_fs.glsl");
  init_a_flock(A_FLOCK);
  init_flock_mesh(BOIDS_MESH, SHADER);
  init_goal_mesh(GOAL_MESH, SHADER);

  glfwGetWindowSize(window, &WIDTH, &HEIGHT);
  PROJ_MAT = glm::perspective(45.0f, WIDTH*1.0f/HEIGHT, 
    CAMERA_NEAR, CAMERA_FAR);
  THE_LIGHT.light0 = glm::vec4(LIGHT_X, LIGHT_Y, LIGHT_Z, 0);
}

void framebuffer_resize(GLFWwindow* window, int width, int height) {
   glViewport(0, 0, width, height);
}

void reshape(GLFWwindow* window, int w, int h) {
  change_view(MV_MAT, VIEW_MODE, A_FLOCK, A_GOAL);
}

void keyboard(GLFWwindow* window, int key, int scancode, int action, int mods) {
  if (action == GLFW_PRESS) {
    switch(key) {
   	  case GLFW_KEY_UP:
      //change_view(ZOOM_IN);
      break;

      case GLFW_KEY_DOWN:
      //change_view(ZOOM_OUT);
      break;

      case GLFW_KEY_F1:
      ENABLE_FLOCK = 1 - ENABLE_FLOCK;
      break;

      case GLFW_KEY_F2:
      ENABLE_GOAL = 1 - ENABLE_GOAL;
      break;

      case GLFW_KEY_F3:
      ENABLE_MOUNTAIN = 1 - ENABLE_MOUNTAIN;
      break;

      case GLFW_KEY_A:
      A_GOAL.MOVE_ALONG_X_NEGATIVE = true;
      break;

      case GLFW_KEY_D:
        A_GOAL.MOVE_ALONG_X_POSITIVE = true;
      break;

      case GLFW_KEY_W:
        A_GOAL.MOVE_ALONG_Y_POSITIVE = true;
      break;

      case GLFW_KEY_S:
        A_GOAL.MOVE_ALONG_Y_NEGATIVE = true;
      break;

      case GLFW_KEY_RIGHT:
        A_GOAL.ACCELERATE = true;
      break;

      case GLFW_KEY_LEFT:
        A_GOAL.DECELERATE = true;
      break;

      case GLFW_KEY_P:
      IS_PAUSED = GLFW_TRUE;
      PAUSE_TIME++;
      break;

      case GLFW_KEY_R:
      IS_PAUSED = GLFW_FALSE;
      PAUSE_TIME = 0;
      break;

      case GLFW_KEY_V:
      VIEW_MODE  = DEFAULT;
      glfwGetWindowSize(window, &WIDTH, &HEIGHT);
      PROJ_MAT = glm::perspective(45.0f, WIDTH*1.0f/HEIGHT, 
        CAMERA_NEAR, CAMERA_FAR);
      break;

      case GLFW_KEY_T:
      VIEW_MODE = TRAILING;
      glfwGetWindowSize(window, &WIDTH, &HEIGHT);
      PROJ_MAT = glm::perspective(30.0f, WIDTH*1.0f/HEIGHT, 
        CAMERA_NEAR, CAMERA_FAR);
      break;

      case GLFW_KEY_G:
      VIEW_MODE = SIDE;
      glfwGetWindowSize(window, &WIDTH, &HEIGHT);
      PROJ_MAT = glm::perspective(40.0f, WIDTH*1.0f/HEIGHT, 
        CAMERA_NEAR, CAMERA_FAR);
      break;

      case GLFW_KEY_Q:
      case GLFW_KEY_ESCAPE:
      glfwSetWindowShouldClose(window, GLFW_TRUE);
      break;
      default:
      break;
    }
  } else if (action == GLFW_RELEASE) {
    switch(key) {
      case GLFW_KEY_A:
      A_GOAL.MOVE_ALONG_X_NEGATIVE = false;
      break;

      case GLFW_KEY_D:
      A_GOAL.MOVE_ALONG_X_POSITIVE = false;
      break;

      case GLFW_KEY_W:
      A_GOAL.MOVE_ALONG_Y_POSITIVE = false;
      break;

      case GLFW_KEY_S:
      A_GOAL.MOVE_ALONG_Y_NEGATIVE = false;

      case GLFW_KEY_RIGHT:
      A_GOAL.ACCELERATE = false;
      break;

      case GLFW_KEY_LEFT:
      A_GOAL.DECELERATE = false;
      break;

      default:
      break;
    }
  }
}
