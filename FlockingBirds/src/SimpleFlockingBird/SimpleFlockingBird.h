#ifndef SIMPLEFLOCKINGBIR_H
#define SIMPLEFLOCKINGBIR_H
#include "src/Algorithm.h"
#include "src/Bird.h"
#include "src/Obstacle.h"
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
