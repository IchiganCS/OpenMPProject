#include "CSVWriter.h"

using namespace std;

template <class T> std::ostream& operator<<(std::ostream& out, std::optional<T> const& opt)
{
    if (opt.has_value())
        out << opt.value();

    return out;
}

void writeHeader(ostream& file)
{
    file << "name, time, threadCount, birdCount, obstacleCount, partitionCount, partitionOverload\n";
}

void writeEntry(ostream& file, CSVEntry const& entry)
{
    file << entry.methodName << ", " << entry.time.count() << ", " << entry.threadCount << ", " << entry.birdCount << ", " << entry.obstacleCount
         << ", " << entry.partitionCount << ", " << entry.partitionOverload << "\n";
}