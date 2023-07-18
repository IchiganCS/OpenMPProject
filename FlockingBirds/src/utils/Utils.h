#ifndef UTILS_H
#define UTILS_H

#include "src/Bird.h"

#include <stdlib.h>
#include <sys/time.h>
#include <time.h>
#include <vector>

class Utils {
public:
  static int WINDOW_WIDTH;
  static int WINDOW_HEIGHT;
  static int SEPRATION_FACTOR;
  static int ALIGNMENT_FACTOR;
  static int COHESION_FACTOR;
  static int FPS;

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
      Vec &pos = b.position;
      b.position.x = getRandomNum(0, WINDOW_WIDTH);
      b.position.y = getRandomNum(0, WINDOW_HEIGHT);
      b.velocity.x = getRandomNum(0, WINDOW_WIDTH);
      b.velocity.y = getRandomNum(0, WINDOW_WIDTH);
      birds.push_back(b);
    }
  }

  static Bird getBird() {
    Bird b;
    b.angle = getRandomNum(0, 360);
    Vec &pos = b.position;
    b.position.x = getRandomNum(0, WINDOW_WIDTH);
    b.position.y = getRandomNum(0, WINDOW_HEIGHT);
    b.velocity.x = getRandomNum(0, WINDOW_WIDTH);
    b.velocity.y = getRandomNum(0, WINDOW_WIDTH);
    return b;
  }

  static void printBirdsData(std::vector<Bird> &birds) {
    printf("Bird Data\n");
    for (Bird b : birds) {
      b.print();
    }
  }
};

#endif