#pragma once

#include "Algorithm.h"
#include "Bird.h"
#include "vector"
#include <array>
#include <memory>

CSVEntry benchmark(Algorithm* algorithm);

void benchmarkAndWriteMany(Algorithm* algorithm, std::ostream& file, int iterationCount, int size, int visionRadius, int leaderCount);