#ifndef UTILS_H
#define UTILS_H

#include "Bird.h"
#include "Vec.h"
#include <vector>

#define sep_radius 40
#define alignment_radius 200
#define collisionCutOff 200.0
#define collisionBreakOne 1000.0

static Vec getAlignment(const std::vector<Bird> &neighbors, const Bird &current,
                        int current_index) {

  size_t num_boids = neighbors.size();
  Vec average_align_velocity;
  int align_neighbours_count = 0;
  for (size_t j = 0; j < num_boids; ++j) {
    if (current_index != j) {
      Vec diff = current.position - neighbors[j].position;
      double distance = diff.length();
      if (distance < alignment_radius) {
        align_neighbours_count++;
        average_align_velocity = average_align_velocity + neighbors[j].velocity;
      }
    }
  }

  if (align_neighbours_count != 0) {
    average_align_velocity =
        average_align_velocity * (1.0 / align_neighbours_count);
  }

  Vec new_align_vec = average_align_velocity - current.velocity;
  new_align_vec = new_align_vec.getNormalisedVec();
  return new_align_vec;
}
static Vec getSeparation(const std::vector<Bird> &neighbors,
                         const Bird &current, int current_index) {

  size_t num_boids = neighbors.size();
  Vec sep_vec;
  int align_neighbours_count = 0;
  for (size_t j = 0; j < num_boids; ++j) {
    if (current_index != j) {
      Vec diff = current.position - neighbors[j].position;
      double distance = diff.length();
      if (distance < sep_radius)
        sep_vec = sep_vec + (diff.getNormalisedVec() * (sep_radius - distance));
    }
  }
  return sep_vec.getNormalisedVec();
}

static Vec getCohesion(const std::vector<Bird> &neighbors, const Bird &current,
                       int current_index) {

  size_t num_boids = neighbors.size();
  Vec average_position;
  ;
  int align_neighbours_count = 0;
  for (size_t j = 0; j < num_boids; ++j) {
    if (current_index != j) {
      Vec diff = current.position - neighbors[j].position;
      double distance = diff.length();
      if (distance < alignment_radius) {
        align_neighbours_count++;
        average_position = average_position + neighbors[j].position;
      }
    }
  }

  if (align_neighbours_count != 0) {
    average_position = average_position * (1.0 / align_neighbours_count);
  }

  Vec new_cohesion_vec = average_position - current.position;
  new_cohesion_vec = new_cohesion_vec.getNormalisedVec();
  return new_cohesion_vec;
}
Vec getAvoidance(const std::vector<Obstacle> &obstacles, const Bird &current);
Vec getLeaderAttraction(const std::vector<Bird> &leaders, const Bird &current);
Vec getGoalAttraction(const Vec &goal, const Bird &leader);
Vec combineForces(const Vec &cohesion, const Vec &alignment,
                  const Vec &separation, const Vec &avoidance,
                  const Vec &goalAttraction, const Vec &leaderAttraction);
void addForce(const Vec &force, Bird &bird);

#endif