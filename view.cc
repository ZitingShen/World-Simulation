#include "view.h"
#include <algorithm>

void change_view(GLfloat mv_mat[], viewMode viewmode, List *flock, GOAL *goal, int index) {
  vec4 centroid = (flock_centroid(flock, 0) + flock_centroid(flock, 1)) * 0.5;
  vec4 midpoint = (mid_point(flock, goal, 0) + mid_point(flock, goal, 1)) * 0.5;
  float distance = (get_d(flock, goal, 0) + get_d(flock, goal, 1)) * 0.5;
  vec4 camera_pos;
  vec4 flock_direction = normalise((get_u(flock, goal, 0) + get_u(flock, goal, 1)) * 0.5);
  vec4 velocity_direction = normalise(get_average_v(flock, index)); 

  GLfloat eye[] = {0, 2500, TOWER_HEIGHT};
  GLfloat centre[] = {midpoint[0], midpoint[1], midpoint[2]};
  GLfloat up[] = {0, 0, 1};

  switch(viewmode) {
    case TRAILING:
    centroid = (flock_centroid(flock, index));
    distance = (get_d(flock, goal, index));
    distance = (distance < 4000)?4000:distance;
    //std::cout << distance << std::endl;
    camera_pos = centroid
                 - flock_direction*distance*0.9
                 - velocity_direction*distance*1.1
                 + vec4(0, 0, 1, 0)*TOWER_HEIGHT*0.8f;
    unpack(camera_pos, eye);
    break;

    case SIDE:{
    vec3 side_v3 = cross(reduce(flock_direction), vec3(0, 0, 1));
    vec4 side_v = side_v3.promote(false);
    camera_pos = centroid
                 + normalise(side_v)*distance*3
                 + vec4(0, 0, 1, 0)*distance*0.3f;
    unpack(camera_pos, eye);
    }
    break;
    default:
    break;
  }
  myLookAt(mv_mat, eye, centre, up);
}
