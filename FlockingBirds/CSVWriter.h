#pragma once


#include <iostream>

void writeHeader(std::ostream& file);

void writeSeqEntry(std::ostream& file, int birdCount, float time);
void writeParEntry(std::ostream& file, int birdCount, float time, int partitionCount);
