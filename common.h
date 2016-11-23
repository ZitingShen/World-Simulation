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
#define TRUE					  1
#define FALSE					  0

#define POS_LOCATION              0
#define NORMAL_LOCATION           1
#define TEXTURE_LOCATION		  2

#define PARTNER_RADIUS            300
#define DEFAULT_FLOCK_SIZE        20
#define MIN_FLOCK_RADIUS	      2000.0
#define MAX_FLOCK_RADIUS		  3000.0

#define BOID_SIZE                 30.0f
#define GOAL_SIZE				  30.0f

#define WORLD_SIZE 				  4096.0f       // edge length of island mesh
#define SUBDIVISIONS              2048           // has to be an even number
#define SUBDIVISIONS_TIMES		  4
#define ISLAND_SIZE				  (WORLD_SIZE/SUBDIVISIONS)
#define PERTUBE_LEVEL 			  5000.0f

#define SEPARATION_WEIGHT         0.002f
#define ALIGNMENT_WEIGHT          0.0008f
#define COHESION_WEIGHT           0.0015f
#define ATTRACTION_WEIGHT         0.0005f
#define STAY_IN_FLOCK_WEIGHT	  0.003f
#define SCATTERING 				  (0.8f*PARTNER_RADIUS)
#define COLLIDING				  (0.2f*PARTNER_RADIUS)
#define FLOCK_RAIUS_CAP			  (10.0f*BOID_SIZE)
#define APPROACHING_GOAL		  (20.0f*GOAL_SIZE)
#define Z_SPEED_CAP				  20.0f
#define BOID_SPEED_FLOOR		  30.0f
#define PREDATOR_SPEED_CAP		  300.0f

#define RANDOMISE_V_FACTOR        60
#define SPAWN_CUBE_LENGTH         100.0f

#define DEFAULT_ACCELERATION_FACTOR    	0.2f
#define DEFAULT_ACCELERATION_MARGIN   	0.1f

#define SUN_POINTS				  10000
#define SUN_RADIUS				  3000.0f
#define SUN_SIZE				  500.0f
#define SUN_MOVEMENT			  (0.4*DEGREE_TO_RADIAN)
#define SHINENESS				  0.1f

#define GOAL_RADIUS    8000.0f
#define GOAL_MOVEMENT  (0.3*DEGREE_TO_RADIAN)

#define OCEAN_SIZE 				  50000
#define ENVIRONMENT_SIZE			  1200.0
#define SEED                      314159265

#define TOWER_INITIAL_HEIGHT	  2000.0f
#define TOWER_ZOOM_STEP			  500.0f
#define TOWER_MIN				  500.0f
#define TOWER_MAX		          4000.0f

#define CAMERA_NEAR               2.0f
#define CAMERA_FAR                100000000.0f

#define ENV_RADIUS    			  3000.0f
#define ENV_MOVEMENT  			  (0.2*DEGREE_TO_RADIAN)

#define BOID_TEXTURE			  "ppms/paper.ppm"
#define GOAL_TEXTURE			  "ppms/fire.ppm"
#define SUN_TEXTURE				  "ppms/fire.ppm"
#define OCEAN_TEXTURE			  "ppms/ocean.ppm"
#define ISLAND_TEXTURE			  "ppms/cliff.ppm"
#define SNOW_TEXTURE			  "ppms/snow.ppm"
#define TREE_TEXTURE		      "ppms/walnut.ppm"
#define ENVIRONMENT_TEXTURE_POS_X "ppms/environment/right.ppm"
#define ENVIRONMENT_TEXTURE_NEG_X "ppms/environment/left.ppm"
#define ENVIRONMENT_TEXTURE_POS_Y "ppms/environment/down.ppm"
#define ENVIRONMENT_TEXTURE_NEG_Y "ppms/environment/up.ppm"
#define ENVIRONMENT_TEXTURE_POS_Z "ppms/environment/back.ppm"
#define ENVIRONMENT_TEXTURE_NEG_Z "ppms/environment/front.ppm"

#define SPHERE_POS_X 500.0f
#define SPHERE_POS_Y 500.0f
#define SPHERE_POS_Z 1000.0f

#define BOID_SPEED_CAP 20.0f

#endif
