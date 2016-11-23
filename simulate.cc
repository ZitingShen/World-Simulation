#include "simulate.h"

using namespace std;

int ENABLE_FLOCK = GLFW_TRUE;
int ENABLE_GOAL = GLFW_TRUE;
int ENABLE_ISLAND = GLFW_TRUE;
int WIDTH, HEIGHT;
int IS_PAUSED = GLFW_FALSE;
int PAUSE_TIME = 0;
int TEXTURE_COUNTER = 0;

glm::mat4 PROJ_MAT, MV_MAT = glm::mat4();
LIGHT THE_LIGHT;
spotlight SPOT_LIGHT;
MESH BOIDS_MESH, GOAL_MESH, SUN_MESH, OCEAN_MESH, SPHERE_MESH;
MESH ISLAND_MESH, TREE_MESH;
vector<glm::vec3> TREE_POS;
vector<vector<GLuint>> ISLAND_INDICES;
vector<GLuint> ISLAND_EBOS;
vector<PREDATOR> PREDATORS;
GLuint SHADER, ENVIRONMENT_SHADER;

camera MY_CAMERA(TOWER_INITIAL_HEIGHT);

vector<BOID> A_FLOCK;
GOAL A_GOAL;

glm::vec3 SUN_POS = glm::vec3(45*DEGREE_TO_RADIAN, 45*DEGREE_TO_RADIAN, 
  45*DEGREE_TO_RADIAN);
glm::vec3 SPHERE_POS = glm::vec3(SPHERE_POS_X, SPHERE_POS_Y, SPHERE_POS_Z);
glm::vec3 ENV_CENTRE = SPHERE_POS;
/*  for steerable spotlight */
mouse MOUSE_STATUS;
double x_movement, y_movement;

int main(int argc, char *argv[]){
  if (!glfwInit ()) {
    std::cerr << "ERROR: could not start GLFW3" << std::endl;
    exit(EXIT_FAILURE);
  }

  glfwWindowHint (GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint (GLFW_CONTEXT_VERSION_MINOR, 2);
  glfwWindowHint (GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
  glfwWindowHint (GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  GLFWwindow* window = glfwCreateWindow(700, 700, "Make a World",
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
  glfwSetCursorPosCallback(window, cursor);
  glfwSetKeyCallback(window, keyboard);
  glfwSetFramebufferSizeCallback(window, framebuffer_resize);

  glEnable(GL_DEPTH_TEST);
  glEnable(GL_CULL_FACE);
  glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);

  while(!glfwWindowShouldClose(window)) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glfwPollEvents();

    MY_CAMERA.change_view(MV_MAT, A_FLOCK, A_GOAL, ISLAND_MESH.center, ENV_CENTRE, 
      SPOT_LIGHT.coneDirection);
    update_spot_light(SPOT_LIGHT,
                      WIDTH, HEIGHT,
                      MV_MAT,
                      PROJ_MAT,
                      MOUSE_STATUS,
                      glm::vec4(A_FLOCK[0].pos, 1), A_FLOCK[0].velocity, MY_CAMERA.current_vm == FP);

    if(!IS_PAUSED || PAUSE_TIME > 0) {
      update_goal_velocity(A_GOAL);
      update_goal_pos(A_GOAL);
      update_velocity(A_FLOCK, A_GOAL);
      apply_goal_attraction(A_FLOCK, A_GOAL);
      apply_predator_deterrence(PREDATORS, A_FLOCK);
      update_pos(A_FLOCK);
      update_sun_pos(SUN_POS);
      update_light(SUN_POS, THE_LIGHT);
      if (IS_PAUSED && PAUSE_TIME > 0) {
        print_goal(A_GOAL);
        print_flock(A_FLOCK);
        PAUSE_TIME--;
      }
    }

    glUseProgram(SHADER);
    if(glfwGetWindowAttrib(window, GLFW_VISIBLE)){
      if (ENABLE_FLOCK)
        draw_a_flock(A_FLOCK, BOIDS_MESH, SHADER, MV_MAT, THE_LIGHT, SPOT_LIGHT, 
          MY_CAMERA.current_vm == FP);
      if (ENABLE_GOAL)
        draw_a_goal(A_GOAL, GOAL_MESH, SHADER, MV_MAT, THE_LIGHT, SPOT_LIGHT);
      if (ENABLE_ISLAND)
        draw_island(ISLAND_MESH, ISLAND_EBOS, SHADER, MV_MAT, THE_LIGHT, SPOT_LIGHT, 
          MY_CAMERA.eye);
      draw_a_sun(SUN_POS, ISLAND_MESH.vertices[ISLAND_MESH.vertices.size()/2].pos, SUN_MESH, 
        SHADER, MV_MAT, THE_LIGHT, SPOT_LIGHT);
      draw_ocean(OCEAN_MESH, SHADER, MV_MAT, THE_LIGHT, SPOT_LIGHT);
      draw_tree(TREE_MESH, SHADER, MV_MAT, THE_LIGHT, SPOT_LIGHT, TREE_POS, 200.0f);

      glUseProgram(ENVIRONMENT_SHADER);
      //draw_environment(SPHERE_MESH, ENVIRONMENT_SHADER, MV_MAT, THE_LIGHT, MY_CAMERA.eye, 
      //  SPOT_LIGHT, SPHERE_POS);
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
  ENVIRONMENT_SHADER = initshader("environmental_vs.glsl", "environmental_fs.glsl");
  glfwGetWindowSize(window, &WIDTH, &HEIGHT);
  PROJ_MAT = glm::perspective(45.0f, WIDTH*1.0f/HEIGHT,
    CAMERA_NEAR, CAMERA_FAR);
  init_a_flock(A_FLOCK);
  initialise_spot_light(SPOT_LIGHT, glm::vec4(A_FLOCK[0].pos, 1), A_FLOCK[0].velocity);
  init_flock_mesh(BOIDS_MESH, SHADER, PROJ_MAT, TEXTURE_COUNTER);
  init_goal_mesh(GOAL_MESH, SHADER, PROJ_MAT, TEXTURE_COUNTER);
  read_mesh("meshes/sphere2.off", SUN_MESH, SHADER, PROJ_MAT);
  init_sun_mesh(SUN_MESH, SHADER, PROJ_MAT, TEXTURE_COUNTER);
  init_ocean_mesh(OCEAN_MESH, SHADER, PROJ_MAT, TEXTURE_COUNTER);
  generate_island_mesh(ISLAND_MESH, ISLAND_INDICES, ISLAND_EBOS, SHADER, PROJ_MAT, 
    TEXTURE_COUNTER);
  create_predators(PREDATORS, ISLAND_MESH);
  read_mesh("meshes/gnarly.off", TREE_MESH, SHADER, PROJ_MAT);
  init_tree_mesh(TREE_MESH, ISLAND_MESH, TREE_TEXTURE, SHADER, 
    PROJ_MAT, TREE_POS, 20, TEXTURE_COUNTER);
  read_mesh("meshes/sphere2.off", SPHERE_MESH, SHADER, PROJ_MAT);
  init_environment_mesh(SPHERE_MESH, ENVIRONMENT_SHADER, PROJ_MAT, TEXTURE_COUNTER);
  glfwGetCursorPos(window, &MOUSE_STATUS.x_pos, &MOUSE_STATUS.y_pos); // get mouse position
 
  MY_CAMERA.current_vm = DEFAULT;
}

void framebuffer_resize(GLFWwindow* window, int width, int height) {
   glViewport(0, 0, width, height);
}

void reshape(GLFWwindow* window, int w, int h) {
  MY_CAMERA.change_view(MV_MAT, A_FLOCK, A_GOAL, ISLAND_MESH.center, ENV_CENTRE, 
    SPOT_LIGHT.coneDirection);
}

void cursor(GLFWwindow* window, double xpos, double ypos){
  glfwGetCursorPos(window, &MOUSE_STATUS.x_pos, &MOUSE_STATUS.y_pos); // get mouse position
}

void keyboard(GLFWwindow* window, int key, int scancode, int action, int mods) {
  if (action == GLFW_PRESS) {
    switch(key) {
      case GLFW_KEY_1:
        teleport_flock(A_FLOCK, A_GOAL, TELEPORT_POS1);
      break;

      case GLFW_KEY_2:
        teleport_flock(A_FLOCK, A_GOAL, TELEPORT_POS2);
      break;

      case GLFW_KEY_3:
        teleport_flock(A_FLOCK, A_GOAL, TELEPORT_POS3);
      break;

   	  case GLFW_KEY_UP:
        MY_CAMERA.zoom_in();
      break;

      case GLFW_KEY_DOWN:
        MY_CAMERA.zoom_out();
      break;

      case GLFW_KEY_N:
        MY_CAMERA.zoom_reset();
      break;

      case GLFW_KEY_F1:
      ENABLE_FLOCK = 1 - ENABLE_FLOCK;
      break;

      case GLFW_KEY_F2:
      ENABLE_GOAL = 1 - ENABLE_GOAL;
      break;

      case GLFW_KEY_F3:
      ENABLE_ISLAND = 1 - ENABLE_ISLAND;
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
        MY_CAMERA.current_vm  = DEFAULT;
        glfwGetWindowSize(window, &WIDTH, &HEIGHT);
        PROJ_MAT = glm::perspective(45.0f, WIDTH*1.0f/HEIGHT,
        CAMERA_NEAR, CAMERA_FAR);
      break;

      case GLFW_KEY_T:
        MY_CAMERA.current_vm = TRAILING;
        glfwGetWindowSize(window, &WIDTH, &HEIGHT);
        PROJ_MAT = glm::perspective(30.0f, WIDTH*1.0f/HEIGHT,
        CAMERA_NEAR, CAMERA_FAR);
      break;

      case GLFW_KEY_G:
        MY_CAMERA.current_vm = SIDE;
        glfwGetWindowSize(window, &WIDTH, &HEIGHT);
        PROJ_MAT = glm::perspective(40.0f, WIDTH*1.0f/HEIGHT,
        CAMERA_NEAR, CAMERA_FAR);
      break;

      case GLFW_KEY_F: //first person view
        MY_CAMERA.current_vm = FP;
        glfwGetWindowSize(window, &WIDTH, &HEIGHT);
        PROJ_MAT = glm::perspective(30.0f, WIDTH*1.0f/HEIGHT,
        CAMERA_NEAR, CAMERA_FAR);
      break;

      case GLFW_KEY_L:
        MY_CAMERA.current_vm = ENV;
        glfwGetWindowSize(window, &WIDTH, &HEIGHT);
        PROJ_MAT = glm::perspective(40.0f, WIDTH*1.0f/HEIGHT,
        CAMERA_NEAR, CAMERA_FAR);
      break;
      
      case GLFW_KEY_O:
        MY_CAMERA.current_vm = FP_TRAILING;
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
      break;

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

