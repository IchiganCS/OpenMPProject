#pragma once


#include <chrono>
#include <iostream>

void writeHeader(std::ostream& file);

struct CSVEntry {
    std::string methodName;
    std::chrono::microseconds time;
    int birdCount;
    int obstacleCount;
    int size;
    int visionRadius;
    int leaderCount;

    int threadCount;
    std::optional<float> partitionOverload;
};

void writeEntry(std::ostream& file, CSVEntry const& entry);
std::ostream& operator<<(std::ostream& file, CSVEntry const& entry);
