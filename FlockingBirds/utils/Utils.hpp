#ifndef UTILS_H
#define UTILS_H

#include "Bird.h"
#include <stdlib.h>
#include <sys/time.h>
#include <time.h>
#include <vector>

class Utils {
public:
  static int getRandomNum(int min, int max) {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    printf("%d\n", tv.tv_usec);
    srand(tv.tv_usec);
    return (rand() % max + min);
  }
  static void intialiseInitPostion(std::vector<Bird> &birds, int noOfBirds) {
    birds.clear();
    for (int i = 0; i < noOfBirds; i++) {
      Bird b;
      b.angle = getRandomNum(0, 360);
      Vec &pos = b.position;
      b.position.X = getRandomNum(0, 100);
      b.position.Y = getRandomNum(0, 100);
      birds.push_back(b);
    }
  }

  static void printBirdsData(std::vector<Bird> &birds) {
    printf("Bird Data\n");
    for (Bird b : birds) {
      b.print();
    }
  }
};

#endif