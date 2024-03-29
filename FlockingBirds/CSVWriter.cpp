#include "CSVWriter.h"

using namespace std;

template <class T> static ostream& operator<<(ostream& out, optional<T> const& opt)
{
    if (opt.has_value())
        out << opt.value();

    return out;
}

void writeHeader(ostream& file)
{
    file << "name, time, birdCount, obstacleCount, leaderCount, visionRadius, size, threadCount, "
            "partitionOverload\n";
}

void writeEntry(ostream& file, CSVEntry const& entry)
{
    file << entry.methodName << ", " << entry.time.count() << ", " << entry.birdCount << ", " << entry.obstacleCount
         << ", " << entry.leaderCount << ", " << entry.visionRadius << ", " << entry.size << ", " << entry.threadCount
         << ", " << entry.partitionOverload << "\n";
}

ostream& operator<<(ostream& file, CSVEntry const& entry)
{
    writeEntry(file, entry);
    return file;
}