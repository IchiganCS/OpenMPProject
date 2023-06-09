#include "csv.hpp"
#include <chrono>

using namespace std;
using namespace chrono;

void writeHeader(ostream &csv)
{
    csv << "multMethod, threadCount, dimension, sparsity, time\n";
}

void writeResult(ostream &csv, const std::string &multMethod, unsigned threadCount, unsigned dimension,
                 float sparsity, microseconds time)
{
    csv << multMethod << ',' << threadCount << ',' << dimension << ',' << sparsity << ',' << time.count() << '\n';
}