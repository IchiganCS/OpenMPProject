#ifndef SIMPLEFLOCKINGBIR_H
#define SIMPLEFLOCKINGBIR_H
#include "Algorithm.h"
#include "Bird.h"
#include "Obstacle.h"
#
#include <vector>

class SimpleFlockingBird : public Algorithm {
private:
  /* data */
public:
  SimpleFlockingBird(/* args */);
  ~SimpleFlockingBird();
  void update(std::vector<Bird> &birds, std::vector<Obstacle> &obs);
};
#endif
