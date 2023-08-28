#ifndef ParallelForAlgoWithRed_HPP
#define ParallelForAlgoWithRed_HPP
#include "Algorithm.h"
#include "Bird.h"
#include "Obstacle.h"
#include "Pointwriter.hpp"
#include "Utils.h"
#include <assert.h>
#include <vector>
#ifdef _OPENMP
#include <omp.h>
#else
#error "OpenMP support required"
#endif

class ParallelForAlgoWithRed : public Algorithm {
private:
  /* data */
public:
  ParallelForAlgoWithRed(/* args */) {}
  ~ParallelForAlgoWithRed() {}
  void boids_algorithm_par_with_reduction(std::vector<Bird> &birds,
                                          std::vector<Obstacle> &obs,
                                          double separation_factor = 1,
                                          double alignment_factor = 1,
                                          double cohesion_factor = 1,
                                          double avoidance_factor = 1) {
    printf(
        "separation factor = %f, alignment factor = %f, cohesion factor = %f, "
        "cohesion factor = %f\n",
        separation_factor, alignment_factor, cohesion_factor, avoidance_factor);
    size_t num_boids = birds.size();

    // Rule 1: Separation - steer to avoid crowding nearby boids
    std::vector<Vec> separation_vectors(num_boids, Vec());
    std::vector<Vec> alignment_vectors(num_boids, Vec());
    std::vector<Vec> cohesion_vectors(num_boids, Vec());
    std::vector<Vec> obstacle_avoidance_vectors(num_boids, Vec());
#pragma omp parallel for num_threads(Utils::NUM_THREADS)
    for (size_t i = 0; i < num_boids; ++i) {
      std::vector<Bird> localThreadCopybirds(birds.begin(), birds.end());
      Vec average_align_velocity;
      Vec average_sepration_velocity;
      int align_neighbours_count = 0;
      Vec average_position;
      float &average_align_velocityx = average_align_velocity.x;
      float &average_align_velocityy = average_align_velocity.y;
      float &average_sepration_velocityx = average_sepration_velocity.x;
      float &average_sepration_velocityy = average_sepration_velocity.y;
      float &average_positionx = average_position.x;
      float &average_positiony = average_position.y;
#pragma omp parallel for num_threads(Utils::NUM_THREADS) \
    reduction(+:average_sepration_velocityx,average_sepration_velocityy,\
    average_align_velocityx,average_align_velocityy,\
    average_positionx,average_positiony) \
    lastprivate (align_neighbours_count)
      for (size_t j = 0; j < num_boids; ++j) {
        if (i != j) {
          Vec diff = localThreadCopybirds[i].position -
                     localThreadCopybirds[j].position;
          double distance = diff.length();
          if (distance < sep_radius)
            average_sepration_velocityx +=
                (diff.getNormalisedVec() * (sep_radius - distance)).x;
          average_sepration_velocityy +=
              (diff.getNormalisedVec() * (sep_radius - distance)).y;
          if (distance < alignment_radius) {
            align_neighbours_count++;
            average_align_velocityx += localThreadCopybirds[j].velocity.x;
            average_align_velocityy += localThreadCopybirds[j].velocity.y;
            average_positionx += localThreadCopybirds[j].position.x;
            average_positiony += localThreadCopybirds[j].position.y;
          }
        }
      }
#pragma omp critical(somename)
      {
        // average_align_velocity.x = average_align_velocityx;
        // average_align_velocity.y = average_align_velocityy;
        // average_sepration_velocity.x = average_sepration_velocityx;
        // average_sepration_velocity.y = average_sepration_velocityy;
        // average_position.x = average_positionx;
        // average_position.y = average_positiony;
        if (align_neighbours_count != 0) {
          average_align_velocity =
              average_align_velocity * (1.0 / align_neighbours_count);
          average_position = average_position * (1.0 / align_neighbours_count);
        }
        alignment_vectors[i] =
            (average_align_velocity - localThreadCopybirds[i].velocity);
        alignment_vectors[i] = alignment_vectors[i].getNormalisedVec();
        cohesion_vectors[i] =
            (average_position - localThreadCopybirds[i].position);
        cohesion_vectors[i] = cohesion_vectors[i].getNormalisedVec();
        separation_vectors[i] = average_sepration_velocity.getNormalisedVec();
      }
      int no_of_close_obs = 0;
      Vec &obs_vec_ref = obstacle_avoidance_vectors[i];
      float &obs_vec_refx = obs_vec_ref.x;
      float &obs_vec_refy = obs_vec_ref.y;
#pragma omp parallel for num_threads(Utils::NUM_THREADS) reduction(+:obs_vec_refx,obs_vec_refy) lastprivate (no_of_close_obs)
      for (int j = 0; j < obs.size(); j++) {
        Obstacle &obstacle = obs[j];
        Vec dir = obstacle.center - localThreadCopybirds[i].position;
        float distLength =
            obstacle.distanceTo(localThreadCopybirds[i].position);

        if (distLength != 0 && distLength < collisionCutOff) {
          dir.toLength(-collisionBreakOne / distLength);
          obs_vec_refx += dir.x;
          obs_vec_refy += dir.y;
          no_of_close_obs++;
        }
      }
      // obs_vec_ref.x = obs_vec_refx;
      // obs_vec_ref.y = obs_vec_refy;
      if (no_of_close_obs != 0) {
        obstacle_avoidance_vectors[i] =
            obstacle_avoidance_vectors[i] * (1.0 / no_of_close_obs);
        obstacle_avoidance_vectors[i] =
            obstacle_avoidance_vectors[i].getNormalisedVec();
      }
    }

#pragma omp barrier

    float total = separation_factor + alignment_factor + cohesion_factor +
                  avoidance_factor;
    // Update velocities and positions
#pragma omp parallel for num_threads(Utils::NUM_THREADS)
    for (size_t i = 0; i < num_boids; ++i) {
      birds[i].velocity =
          birds[i].velocity +
          (separation_vectors[i] * (separation_factor / total)) +
          (alignment_vectors[i] * (alignment_factor / total)) +
          (cohesion_vectors[i] * (cohesion_factor / total)) +
          (obstacle_avoidance_vectors[i] * (avoidance_factor / total));
      birds[i].velocity = birds[i].velocity.getNormalisedVec() * 4;
      birds[i].position = birds[i].position + birds[i].velocity;

      birds[i].position.x =
          wrapAround(birds[i].position.x, 0, Utils::WINDOW_HEIGHT);
      birds[i].position.y =
          wrapAround(birds[i].position.y, 0, Utils::WINDOW_HEIGHT);
      birds[i].angle = birds[i].calculateDirection();

      assert(birds[i].position.x >= 0 &&
             birds[i].position.x <= Utils::WINDOW_HEIGHT);
      assert(birds[i].position.y >= 0 &&
             birds[i].position.y <= Utils::WINDOW_HEIGHT);
    }
  }
  void update(std::vector<Bird> &birds, std::vector<Obstacle> &obs);
};
#endif
