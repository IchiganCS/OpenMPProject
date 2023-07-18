#include "SimpleFlockingBird.h"

#include "src/utils/Utils.h"
#include <assert.h>

static int sep_radius = 40;
static int alignment_radius = 200;

namespace {

double wrapAround(double value, double min, double max) {
  double range = max - min;
  double wrappedValue = value;

  while (wrappedValue < min) {
    wrappedValue += range;
  }

  while (wrappedValue >= max) {
    wrappedValue -= range;
  }

  return wrappedValue;
}

void boids_algorithm(std::vector<Bird> &birds, double separation_factor = 1,
                     double alignment_factor = 1, double cohesion_factor = 1) {
  printf(
      "separation factor = %f, alignment factor = %f, cohesion factor = %f\n",
      separation_factor, alignment_factor, cohesion_factor);
  size_t num_boids = birds.size();

  // Rule 1: Separation - steer to avoid crowding nearby boids
  std::vector<Vec> separation_vectors(num_boids, Vec());
  std::vector<Vec> alignment_vectors(num_boids, Vec());
  std::vector<Vec> cohesion_vectors(num_boids, Vec());
  for (size_t i = 0; i < num_boids; ++i) {
    Vec average_align_velocity;
    int align_neighbours_count = 0;
    Vec average_position;
    for (size_t j = 0; j < num_boids; ++j) {
      if (i != j) {
        Vec diff = birds[i].position - birds[j].position;
        double distance = diff.length();
        if (distance < sep_radius)
          separation_vectors[i] =
              separation_vectors[i] +
              (diff.getNormalisedVec() * (sep_radius - distance));
        if (distance < alignment_radius) {
          average_align_velocity = average_align_velocity + birds[j].velocity;
          average_position = average_position + birds[j].position;
        }
      }
    }
    if (align_neighbours_count != 0) {
      average_align_velocity =
          average_align_velocity * (1.0 / align_neighbours_count);
      average_position = average_position * (1.0 / align_neighbours_count);
    }
    alignment_vectors[i] = (average_align_velocity - birds[i].velocity);
    alignment_vectors[i] = alignment_vectors[i].getNormalisedVec();
    cohesion_vectors[i] = (average_position - birds[i].position);
    cohesion_vectors[i] = cohesion_vectors[i].getNormalisedVec();
    separation_vectors[i] = separation_vectors[i].getNormalisedVec();
  }

  // // Rule 2: Alignment - steer towards the average heading of nearby boids
  // Vec average_velocity;
  // for (size_t i = 0; i < num_boids; ++i) {
  //   average_velocity = average_velocity + birds[i].velocity;
  // }
  // average_velocity = average_velocity * (1.0 / num_boids);
  // std::vector<Vec> alignment_vectors(num_boids, Vec());
  // for (size_t i = 0; i < num_boids; ++i) {
  //   alignment_vectors[i] =
  //       (average_velocity - birds[i].velocity) * alignment_factor;
  // }

  // // Rule 3: Cohesion - steer to move towards the average position of nearby
  // // boids
  // Vec average_position;
  // for (size_t i = 0; i < num_boids; ++i) {
  //   average_position = average_position + birds[i].position;
  // }
  // average_position = average_position * (1.0 / num_boids);
  // std::vector<Vec> cohesion_vectors(num_boids, Vec());
  // for (size_t i = 0; i < num_boids; ++i) {
  //   cohesion_vectors[i] =
  //       (average_position - birds[i].position) * cohesion_factor;
  // }

  float total = separation_factor + alignment_factor + cohesion_factor;
  // Update velocities and positions
  for (size_t i = 0; i < num_boids; ++i) {
    birds[i].velocity = birds[i].velocity +
                        (separation_vectors[i] * (separation_factor / total)) +
                        (alignment_vectors[i] * (alignment_factor / total)) +
                        (cohesion_vectors[i] * (cohesion_factor / total));
    birds[i].velocity = birds[i].velocity.getNormalisedVec() * 4;
    birds[i].position = birds[i].position + birds[i].velocity;

    birds[i].position.x =
        wrapAround(birds[i].position.x, 0, Utils::WINDOW_HEIGHT);
    birds[i].position.y =
        wrapAround(birds[i].position.y, 0, Utils::WINDOW_HEIGHT);
    birds[i].angle = birds[i].calculateDirection();

    // if (!(birds[i].position.x >= 0 &&
    //       birds[i].position.x <= Utils::WINDOW_HEIGHT)) {
    //   birds[i].position.x = int(birds[i].position.x) % Utils::WINDOW_HEIGHT;
    // }
    // if (!(birds[i].position.y >= 0 &&
    //       birds[i].position.y <= Utils::WINDOW_HEIGHT)) {
    //   birds[i].position.y = int(birds[i].position.y) % Utils::WINDOW_HEIGHT;
    // }

    assert(birds[i].position.x >= 0 &&
           birds[i].position.x <= Utils::WINDOW_HEIGHT);
    assert(birds[i].position.y >= 0 &&
           birds[i].position.y <= Utils::WINDOW_HEIGHT);
  }
}
} // namespace

SimpleFlockingBird::SimpleFlockingBird(/* args */) {}

SimpleFlockingBird::~SimpleFlockingBird() {}

void SimpleFlockingBird::update(std::vector<Bird> &birds) {
  Utils::printBirdsData(birds);
  boids_algorithm(birds, Utils::ALIGNMENT_FACTOR, Utils::SEPRATION_FACTOR,
                  Utils::COHESION_FACTOR);
  return;
}
