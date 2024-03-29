#ifndef SIMULATE_H
#define SIMULATE_H

#include <iostream>
#include <vector>
#include <map>
#include <string>
#include "initshader.h"
#include "boid.h"
#include "goal.h"
#include "fin.h"
#include "view.h"
#include "common.h"
#include "sun.h"
#include "ocean.h"
#include "environment.h"
#include "island.h"
#include "light.h"
#include "anti_collision.h"
#include "tree.h"

#include <glm/gtc/matrix_transform.hpp>

const glm::vec3 TELEPORT_POS1 = glm::vec3(500.0f, 500.0f, 3000.0f); // near mountain
const glm::vec3 TELEPORT_POS2 = glm::vec3(5000.0f, 5000.0f, 3000.0f); // away from the mountain
const glm::vec3 TELEPORT_POS3 = glm::vec3(0.0f, 0.0f, 20000.0f); // on the top of the mountain

enum _z_direction{NONE, ZOOM_IN, ZOOM_OUT};

void init(GLFWwindow* window);
void reshape(GLFWwindow* window, int w, int h);
void framebuffer_resize(GLFWwindow* window, int width, int height);
void keyboard(GLFWwindow *w, int key, int scancode, int action, int mods);
void cursor(GLFWwindow* window, double xpos, double ypos);

#endif
