#include "SimpleFlockingBird.h"

#include "src/utils/Utils.h"

static int incx = 1;
static int incy = 1;

SimpleFlockingBird::SimpleFlockingBird(/* args */) {}

SimpleFlockingBird::~SimpleFlockingBird() {}

void SimpleFlockingBird::update(std::vector<Bird> &birds) {
  Utils::printBirdsData(birds);
  for (Bird &b : birds) {
    std::vector<Bird> neighbours;
    std::vector<Bird> tooclose;
    Vec cohesion;
    Vec sepration;
    for (Bird &others : birds) {
      if (b.position.X != others.position.X ||
          b.position.Y != others.position.Y) {
        if (Vec::distance(b.position, others.position) < 10) {
          neighbours.push_back(others);
          cohesion.X += b.position.X - others.position.X;
          cohesion.Y += b.position.Y - others.position.Y;
        }
        if (Vec::distance(b.position, others.position) < 3) {
          tooclose.push_back(others);
          sepration.X += b.position.X - others.position.X;
          sepration.Y += b.position.Y - others.position.Y;
        }
      }
    }

    if (neighbours.size() > 0) {
      cohesion.X /= neighbours.size();
      cohesion.Y /= neighbours.size();
    }
    if (tooclose.size() > 0) {
      sepration.X /= tooclose.size();
      sepration.Y /= tooclose.size();
    }
    int x = b.position.X;
    int y = b.position.Y;
    b.position.X += (0.2 * cohesion.X + sepration.X);
    b.position.Y += (0.2 * cohesion.Y + sepration.Y);
    if (b.position.X == x && b.position.Y == y) {
      b.position.X++;
      b.position.Y++;
    }
    b.position.X = int(b.position.X) % (Utils::WINDOW_WIDTH);
    b.position.Y = int(b.position.Y) % (Utils::WINDOW_HEIGHT);
  }
}