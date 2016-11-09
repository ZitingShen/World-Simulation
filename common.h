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
#define	DEGREE_TO_RADIAN		  0.00873f
#define SQRT_2					  1.414213f

#define CAMERA_NEAR               2.0f
#define CAMERA_FAR                100000000.0f

#define POS_LOCATION              0
#define NORMAL_LOCATION           1

#define PARTNER_RADIUS            300
#define DEFAULT_FLOCK_SIZE        100
#define DEFAULT_FLOCK_NUM         2
#define MIN_FLOCK_RADIUS	      2000.0
#define MAX_FLOCK_RADIUS		  3000.0

#define SEPARATION_WEIGHT         0.002f
#define ALIGNMENT_WEIGHT          0.0008f
#define COHESION_WEIGHT           0.0015f
#define ATTRACTION_WEIGHT         0.0005f
#define STAY_IN_FLOCK_WEIGHT	  0.003f
#define DETERRENCE_WEIGHT         0.1f // the most significant weight

#define MAX_WING_ROTATION         45
#define WING_ROTATION_PER_FRAME   0.01f
#define SHADES_HEIGHT             50

#define RANDOMISE_V_FACTOR        60
#define SPAWN_CUBE_LENGTH         100.0f

#define BOID_SIZE                 100.0f
#define GOAL_SIZE				  100.0f

#define DEFAULT_ACCELERATION_FACTOR    	0.2f
#define DEFAULT_ACCELERATION_MARGIN   	0.1f
#endif