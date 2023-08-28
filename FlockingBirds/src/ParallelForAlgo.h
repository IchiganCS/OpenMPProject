#ifndef ParallelForAlgo_H
#define ParallelForAlgo_H
#include "Algorithm.h"
#include "Bird.h"
#include "Obstacle.h"
#
#include <vector>

class ParallelForAlgo : public Algorithm {
private:
  /* data */
public:
  ParallelForAlgo(/* args */);
  ~ParallelForAlgo();
  void update(std::vector<Bird> &birds, std::vector<Obstacle> &obs);
};
#endif
