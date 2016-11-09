#ifndef COMMON_H
#define COMMON_H

#ifdef __APPLE__
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <OpenGL/glu.h>
#else
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <GL/glu.h>
#endif

#include <time.h>
#include <glm/glm.hpp>
#include <glm/gtx/string_cast.hpp>
#include <glm/gtc/type_ptr.hpp>

#define RADIAN_TO_DEGREE          57.29578f // 180 over pi
#define	DEGREE_TO_RADIAN		      0.00873f

#define CAMERA_NEAR               0.1f
#define CAMERA_FAR                100000000.0f

#define POS_LOCATION              0
#define NORMAL_LOCATION           1

#endif