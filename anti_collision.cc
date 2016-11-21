#include "anti_collision.h"


void create_predators(vector<PREDATOR>& predators, vector<MESH>& obstacles){
  predators.resize(obstacles.size());
  for (int i=0; i<(int)obstacles.size(); i++){
    predators[i].pos = obstacles[i].center;
    predators[i].deterrence_range = RANGE;
  }
}

void apply_predator_deterrence(vector<PREDATOR>& predators, vector<BOID> a_flock){
  glm::vec3 dis_to_predator;
  for (int i=0; i<(int)a_flock.size();i++){ 
  if (glm::distance(a_flock[i].pos, predators[i].pos) < predators[i].deterrence_range){
     dis_to_predator = a_flock[i].pos - predators[i].pos;
     a_flock[i].velocity += dis_to_predator * DETERRENCE_WEIGHT;
   }
  }
}


