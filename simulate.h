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
#include "octopus.h"
#include "island.h"

#include <glm/gtc/matrix_transform.hpp>

enum _z_direction{NONE, ZOOM_IN, ZOOM_OUT};

void init(GLFWwindow* window);
void reshape(GLFWwindow* window, int w, int h);
void framebuffer_resize(GLFWwindow* window, int width, int height);
void keyboard(GLFWwindow *w, int key, int scancode, int action, int mods);

#endif
