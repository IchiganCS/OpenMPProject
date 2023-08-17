#include "Benchmark.h"
#include "Bird.h"
#include "CSVWriter.h"
#include "Draw.h"
#include <chrono>

void benchmark(Algorithm* algorithm, int iterationCount, int threadCount, std::ostream& output)
{
    for (int i = 0; i < iterationCount; i++)
    {
        auto start = std::chrono::high_resolution_clock::now();
        algorithm->update();
        auto end = std::chrono::high_resolution_clock::now();

        CSVEntry entry;
        algorithm->fillCSV(entry);
        entry.threadCount = threadCount;
        entry.time = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
        writeEntry(output, entry);
    }
}