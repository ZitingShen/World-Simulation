#include "view.h"

void change_view(glm::mat4& MV_MAT,
                 viewMode vm,
                 float tower,
                 std::vector<BOID>& flock,
                 GOAL& goal,
                 glm::vec3& island_centre,
                 glm::vec3& eye){
  glm::vec3 centroid = flock_centroid(flock);
  //glm::vec3 midpoint = mid_point(flock, goal);
  float distance = get_d(flock, goal);

  glm::vec3 flock_direction = glm::normalize(get_u(flock, goal));
  glm::vec3 velocity_direction = glm::normalize(get_average_v(flock));

  eye = glm::vec3(0, 0, tower);
  //glm::vec3 center = glm::vec3(midpoint[0], midpoint[1], midpoint[2]);
  //cout << island_centre.x << " " << island_centre.y << " " << island_centre.z << endl;
  glm::vec3 center = glm::vec3(island_centre.x, 0.1f, island_centre.z);
  glm::vec3 up = glm::vec3(0, 0, 1);
  //glm::vec3 sp_pos = flock[0].pos + normalize(flock[0].velocity) * BOID_SIZE;
  float t = (0.0 - flock[0].pos.z) / flock[0].velocity.z;
  //float t_cone = (0.0 - sp_pos.z) / fp_direction.z;

  glm::vec3 side_v = glm::cross(flock_direction, up);

  switch(vm){
    case TRAILING:
      centroid = flock_centroid(flock);
      distance = get_d(flock, goal);
      distance = (distance < 800)?800:distance;
      eye = centroid
          - flock_direction*distance*1.0f
          - velocity_direction*distance*1.0f
          + up*tower*0.8f;
      break;

    case SIDE:
      eye = centroid
          + glm::normalize(side_v)*distance*(tower/2000.0f)*3.0f
          + up*distance*0.3f;
      center = centroid;
      break;

    case FP:
      centroid = flock[0].pos;
      up = up;
      distance = 2.0f * BOID_SIZE;
      // looking at the interscetion point with the sea plane
       //center = glm::vec3(sp_pos.x + t_cone*fp_direction.x,
       //                  sp_pos.y + t_cone*fp_direction.y,
       //                  0);
      // cout << fp_direction.x << " " << fp_direction.y << " " << fp_direction.z << endl;
      //center = glm::vec3(flock[0].pos.x + t*flock[0].velocity.x,
      //                   flock[0].pos.y + t*flock[0].velocity.y,
      //                   0);
      eye = centroid
          + glm::normalize(flock[0].velocity) * distance * (tower/500.0f);

      break;

    case TRAIL_FP:
      centroid = flock[0].pos;
      up = up;
      // looking at the interscetion point with the sea plane
      center = glm::vec3(flock[0].pos.x + t*flock[0].velocity.x,
                         flock[0].pos.y + t*flock[0].velocity.y,
                         0);

      distance = 10 * BOID_SIZE;
      eye = centroid
          - (glm::normalize(flock[0].velocity) * distance);
      break;

    default:
    break;
  }
  MV_MAT = glm::lookAt(eye, center, up);
}


void zoom_in(float& tower){
  tower = tower==TOWER_MIN?TOWER_MIN:tower-TOWER_ZOOM_STEP;
}

void zoom_out(float& tower){
  tower = tower==TOWER_MAX?TOWER_MAX:tower+TOWER_ZOOM_STEP;
}

void zoom_reset(float& tower) {
  tower = TOWER_INITIAL_HEIGHT;
}
