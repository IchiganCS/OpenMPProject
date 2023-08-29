#include "Benchmark.h"
#include "Bird.h"
#include "CSVWriter.h"
#include "Draw.h"
#include <chrono>

CSVEntry benchmark(Algorithm* algorithm)
{
    CSVEntry entry;
    algorithm->fillCSV(entry);
    
    std::cout << "Benchmarking for " << entry.birdCount << "birds, and the method: " << entry.methodName << std::endl;

    auto start = std::chrono::high_resolution_clock::now();
    algorithm->update();
    auto end = std::chrono::high_resolution_clock::now();
    entry.time = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    return entry;
}

void benchmarkAndWriteMany(Algorithm* algorithm, std::ostream& file, int iterationCount, int size, int visionRadius, int leaderCount) {
    for (int i = 0; i < iterationCount; i++) {
        auto entry = benchmark(algorithm);
        entry.size = size;
        entry.visionRadius = visionRadius;
        entry.leaderCount = leaderCount;

        file << entry;
    }
}