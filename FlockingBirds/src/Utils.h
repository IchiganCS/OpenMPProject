#ifndef UTILS_H
#define UTILS_H

#include "Bird.h"

#include "Obstacle.h"
#include "Pointwriter.hpp"
#include <assert.h>
#include <cmath>
#include <stdlib.h>
#include <sys/time.h>
#include <time.h>
#include <vector>

#define sep_radius 40
#define alignment_radius 200
#define collisionCutOff 200.0
#define collisionBreakOne 1000.0

class Utils {
public:
  static int WINDOW_WIDTH;
  static int WINDOW_HEIGHT;
  static int DETAILS_WIDTH;
  static int SEPRATION_FACTOR;
  static int ALIGNMENT_FACTOR;
  static int COHESION_FACTOR;
  static int AVOIDANCE_FACTOR;
  static int FPS;
  static int NUM_THREADS;
  static bool PRINTLOG;
  static PointWriter csv;

  static int getRandomNum(int min, int max) {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    srand(tv.tv_usec);
    return (rand() % max + min);
  }
  static void intialiseInitPostion(std::vector<Bird> &birds, int noOfBirds) {
    birds.clear();
    for (int i = 0; i < noOfBirds; i++) {
      Bird b;
      b.angle = getRandomNum(0, 360);
      b.position.x = getRandomNum(0, WINDOW_WIDTH);
      b.position.y = getRandomNum(0, WINDOW_HEIGHT);
      b.velocity.x = getRandomNum(0, WINDOW_WIDTH);
      b.velocity.y = getRandomNum(0, WINDOW_WIDTH);
      birds.push_back(b);
    }
  }

  static void intialiseInitObstacles(std::vector<Obstacle> &obstacles,
                                     int noOfobstacles) {
    obstacles.clear();
    for (int i = 0; i < noOfobstacles; i++) {
      obstacles.push_back(getObstacle());
    }
  }

  static Bird getBird() {
    Bird b;
    b.angle = getRandomNum(0, 360);
    b.position.x = getRandomNum(0, WINDOW_HEIGHT);
    b.position.y = getRandomNum(0, WINDOW_HEIGHT);
    b.velocity.x = getRandomNum(0, WINDOW_HEIGHT);
    b.velocity.y = getRandomNum(0, WINDOW_HEIGHT);
    return b;
  }

  static Obstacle getObstacle() {
    Obstacle obstacle;
    obstacle.center.x = getRandomNum(0, WINDOW_HEIGHT);
    obstacle.center.y = getRandomNum(0, WINDOW_HEIGHT);
    obstacle.radius = getRandomNum(10, 50);
    return obstacle;
  }

  static void printBirdsData(std::vector<Bird> &birds) {
    if (Utils::PRINTLOG) {
      printf("Bird Data\n");
      for (Bird b : birds) {
        b.print();
      }
    }
    // printf("Bird Data Array\n");
    // Bird *temp = birds.data();
    // int n = birds.size();
    // for (int i = 0; i < n; i++) {
    //   /* code */
    //   Bird &ref = temp[i];
    //   ref.print();
    // }
  }

  static void assertBirdDataEqual(std::vector<Bird> &birds1,
                                  std::vector<Bird> &birds2) {
    assert(birds1.size() == birds2.size());
    int n = birds1.size();
    //#pragma omp parallel for num_threads(Utils::NUM_THREADS)
    for (int i = 0; i < n; i++) {
#define MAXERROR 0.01
      Vec posDiff = birds1[i].position - birds2[i].position;
      Vec velDiff = birds1[i].velocity - birds2[i].velocity;
      float anglediff = birds1[i].angle - birds2[i].angle;
      assert((abs(posDiff.x) < MAXERROR));
      assert((abs(posDiff.y) < MAXERROR));
      assert((abs(velDiff.x) < MAXERROR));
      assert((abs(velDiff.y) < MAXERROR));
      assert((abs(anglediff) < MAXERROR));
    }
  }

  static Vec getAlignment(const std::vector<Bird> &neighbors,
                          const Bird &current, int current_index) {

    size_t num_boids = neighbors.size();
    Vec average_align_velocity;
    int align_neighbours_count = 0;
    for (size_t j = 0; j < num_boids; ++j) {
      if (current_index != j) {
        Vec diff = current.position - neighbors[j].position;
        double distance = diff.length();
        if (distance < alignment_radius) {
          align_neighbours_count++;
          average_align_velocity =
              average_align_velocity + neighbors[j].velocity;
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
    for (size_t j = 0; j < num_boids; ++j) {
      if (current_index != j) {
        Vec diff = current.position - neighbors[j].position;
        double distance = diff.length();
        if (distance < sep_radius)
          sep_vec =
              sep_vec + (diff.getNormalisedVec() * (sep_radius - distance));
      }
    }
    return sep_vec.getNormalisedVec();
  }

  static Vec getCohesion(const std::vector<Bird> &neighbors,
                         const Bird &current, int current_index) {

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
};

#endif