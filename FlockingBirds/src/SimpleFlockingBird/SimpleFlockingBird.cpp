#include "SimpleFlockingBird.h"

#include "src/utils/Utils.h"

SimpleFlockingBird::SimpleFlockingBird(/* args */) {}

SimpleFlockingBird::~SimpleFlockingBird() {}

void SimpleFlockingBird::update(std::vector<Bird> &birds) {
  Utils::incBirdsPos(birds);
}