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
      b.position.X = WINDOW_WIDTH / 2;  // getRandomNum(0, WINDOW_WIDTH);
      b.position.Y = WINDOW_HEIGHT / 2; // getRandomNum(0, WINDOW_HEIGHT);
      b.velocity.X = 1;                 // getRandomNum(-3, 3);
      b.velocity.Y = 1;                 // getRandomNum(-3, 3);
      birds.push_back(b);
    }
  }

  static void printBirdsData(std::vector<Bird> &birds) {
    printf("Bird Data\n");
    for (Bird b : birds) {
      b.print();
    }
  }

  static void incBirdsPos(std::vector<Bird> &birds) {
    for (Bird &b : birds) {
      b.position.X++;
      if (b.position.X > WINDOW_WIDTH) {
        b.position.X = getRandomNum(0, WINDOW_WIDTH);
      }
      b.position.Y++;
      if (b.position.Y > WINDOW_HEIGHT) {
        b.position.Y = getRandomNum(0, WINDOW_HEIGHT);
      }
    }
  }
};

#endif