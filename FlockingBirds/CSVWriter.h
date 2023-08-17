#pragma once


#include <chrono>
#include <iostream>

void writeHeader(std::ostream& file);

struct CSVEntry {
    std::string methodName;
    std::chrono::microseconds time;
    int threadCount;
    int birdCount;
    int obstacleCount;

    std::optional<int> partitionCount;
    std::optional<float> partitionOverload;
};

void writeEntry(std::ostream& file, CSVEntry const& entry);
