#include "ParallelForAlgoWithRed.h"

void ParallelForAlgoWithRed::update(std::vector<Bird> &birds,
                                    std::vector<Obstacle> &obs) {
  boids_algorithm_par_with_reduction(
      birds, obs, Utils::SEPRATION_FACTOR, Utils::ALIGNMENT_FACTOR,
      Utils::COHESION_FACTOR, Utils::AVOIDANCE_FACTOR);
}