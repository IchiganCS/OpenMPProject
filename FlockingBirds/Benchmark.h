#pragma once

#include "Algorithm.h"
#include "Bird.h"
#include "vector"
#include <array>
#include <memory>

void benchmark(Algorithm* algorithm, int iterationCount, int threadCount, std::ostream& output);
