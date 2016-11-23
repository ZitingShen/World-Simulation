#include "view.h"

camera::camera(float tower){
  /* defualt view setup */
  this->eye = glm::vec3(0, 0, 30000); //glm::vec3(0, 0, tower);
  this->up = glm::vec3(0, 0, 1);
  this->center = glm::vec3(0, 0.1f, 0);
  this->env_cam_pos = glm::vec3(0, 0, 5000);
  this->current_vm = DEFAULT;
  this->tower = tower;
}

void camera::change_view(glm::mat4& MV_MAT,
                         std::vector<BOID>& flock,
                         GOAL& goal,
                         glm::vec3& island_centre,
                         glm::vec3& env_centre,
                         glm::vec3& cone_direction){

  glm::vec3 centroid;
  glm::vec3 camera_movement;
  float distance;
  float t_cone;

  glm::vec3 flock_direction = glm::normalize(get_u(flock, goal));
  glm::vec3 velocity_direction = glm::normalize(get_average_v(flock));
  glm::vec3 side_v = glm::cross(flock_direction, up);
  glm::vec3 sp_pos = flock[0].pos + normalize(flock[0].velocity) * BOID_SIZE;

  switch(this->current_vm){
    case DEFAULT:
      centroid = flock_centroid(flock);
      this->eye = glm::vec3(0, 0, tower);
      this->up  = glm::vec3(0, 0, 1);
      this->center = glm::vec3(island_centre.x, 0.1f, island_centre.z);
      break;

    case TRAILING:
      distance = 800.0f;
      centroid = flock_centroid(flock);
      this->eye = centroid
                  - flock_direction*distance*1.0f
                  - velocity_direction*distance*1.0f;
                  + this->up*this->tower*0.8f;
      this->up  = glm::vec3(0, 0, 1);
      this->center = centroid;
      break;

    case SIDE:
      centroid = flock_centroid(flock);
      distance = get_d(flock, goal);
      this->eye = centroid
          + glm::normalize(side_v)*distance*(this->tower/2000.0f)*3.0f
          + this->up*distance*0.3f;
      this->up = glm::vec3(0, 0, 1);
      this->center = centroid;
      break;

    case FP:
      centroid = flock[0].pos;
      distance = 2.0f * BOID_SIZE;
      // looking at the interscetion point with the sea plane
      t_cone = (0.0 - sp_pos.z) / cone_direction.z;

      camera_movement = glm::vec3(sp_pos.x + t_cone*cone_direction.x,
                         sp_pos.y + t_cone*cone_direction.y,
                         0) - this->center;

      if (glm::length(camera_movement) > CAMERA_SPEED_CAP)
        camera_movement = glm::normalize(camera_movement) * CAMERA_SPEED_CAP;

      this->eye = centroid
                + glm::normalize(flock[0].velocity)*distance*(this->tower/500.0f);
      this->up = glm::vec3(0, 0, 1);
      this->center = center + camera_movement;
      break;
    case ENV:
      this->env_cam_pos[0] += ENV_MOVEMENT;
      this->env_cam_pos[1] += ENV_MOVEMENT;
      this->eye = glm::vec3(ENV_RADIUS*glm::cos(this->env_cam_pos[0]),
                            ENV_RADIUS*glm::sin(this->env_cam_pos[1]),
                            this->env_cam_pos[2]);
      this->up = glm::vec3(0, 0, 1);
      this->center = env_centre;

    default:
      break;
  }
  MV_MAT = glm::lookAt(this->eye, this->center, this->up);
}


void camera::zoom_in(){
  this->tower = this->tower==TOWER_MIN?TOWER_MIN:this->tower-TOWER_ZOOM_STEP;
}

void camera::zoom_out(){
  this->tower = this->tower==TOWER_MAX?TOWER_MAX:this->tower+TOWER_ZOOM_STEP;
}

void camera::zoom_reset() {
  this->tower = TOWER_INITIAL_HEIGHT;
}
