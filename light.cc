#include "light.h"

using namespace std;

void update_light(glm::vec3& sun_pos, LIGHT& THE_LIGHT) {
	if (glm::sin(sun_pos[2])*SUN_RADIUS + SUN_SIZE < 0) {
		THE_LIGHT.ifNight = 1;
	} else {
		THE_LIGHT.ifNight = 0;
	}
}