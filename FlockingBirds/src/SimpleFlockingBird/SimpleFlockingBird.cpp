#include "SimpleFlockingBird.h"

#include "src/utils/Utils.h"

static int incx = 1;
static int incy = 1;

SimpleFlockingBird::SimpleFlockingBird(/* args */) {}

SimpleFlockingBird::~SimpleFlockingBird() {}

void SimpleFlockingBird::update(std::vector<Bird> &birds) {
  Utils::printBirdsData(birds);
  for (Bird &b : birds) {

    if (b.position.X > static_cast<float>(Utils::WINDOW_WIDTH - 10)) {
      b.position.X = 10;
    } else if (b.position.X < 10) {
      b.position.X = Utils::WINDOW_WIDTH - 10;
    }

    if (b.position.Y > static_cast<float>(Utils::WINDOW_HEIGHT - 10)) {
      b.position.Y = 10;
    } else if (b.position.Y < 10) {
      b.position.Y = Utils::WINDOW_HEIGHT - 10;
    }

    std::vector<Bird> neighbours;
    std::vector<Bird> tooclose;
    Vec cohesion;
    Vec sepration;
    Vec alignment;
    for (Bird &others : birds) {
      // if (b != others) {
      if (Vec::distance(b.position, others.position) < 1000) {
        neighbours.push_back(others);
        cohesion.X += others.position.X;
        cohesion.Y += others.position.Y;
        alignment.X += others.velocity.X;
        alignment.Y += others.velocity.Y;
      }
      if (Vec::distance(b.position, others.position) < 700) {
        tooclose.push_back(others);
        sepration.X += (b.position.X - others.position.X) /
                       (Vec::distance(b.position, others.position) == 0
                            ? 1
                            : Vec::distance(b.position, others.position));
        sepration.Y += (b.position.Y - others.position.Y) /
                       (Vec::distance(b.position, others.position) == 0
                            ? 1
                            : Vec::distance(b.position, others.position));
      }
      //}
    }

    if (neighbours.size() > 1) {
      cohesion.X /= neighbours.size() - 1;
      cohesion.Y /= neighbours.size() - 1;
      cohesion.X -= b.position.X;
      cohesion.Y -= b.position.Y;
      cohesion.X -= b.position.X;
      cohesion.Y -= b.position.Y;
      cohesion = cohesion.getNormalisedVec();
      alignment.X -= b.velocity.X;
      alignment.Y -= b.velocity.Y;
      alignment.X /= neighbours.size() - 1;
      alignment.Y /= neighbours.size() - 1;
      alignment = alignment.getNormalisedVec();
    }
    if (tooclose.size() > 1) {
      sepration.X /= tooclose.size() - 1;
      sepration.Y /= tooclose.size() - 1;
      sepration = sepration.getNormalisedVec();
    }
    int x = b.position.X;
    int y = b.position.Y;
    b.velocity.X += (0.2 * cohesion.X + 0.8 * alignment.X + sepration.X);
    b.velocity.Y += (0.2 * cohesion.Y + 0.8 * alignment.X + sepration.Y);
    b.velocity = b.velocity.getNormalisedVec();
    b.position.X = b.position.X + b.velocity.X;
    b.position.Y = b.position.Y + b.velocity.Y;
  }
}