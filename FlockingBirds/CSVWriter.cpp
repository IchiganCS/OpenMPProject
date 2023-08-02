#include "CSVWriter.h"

using namespace std;

void writeHeader(ostream& file)
{
    file << "name, birdCount, time, partitionCount\n";
}

void writeSeqEntry(ostream& file, int birdCount, float time)
{
    file << "sequential, " << birdCount << ", " << time << ", \n";
}
void writeParEntry(ostream& file, int birdCount, float time, int partitionCount)
{
    file << "parallel, " << birdCount << ", " << time << ", " << partitionCount << "\n";
}
