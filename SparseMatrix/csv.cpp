#include "csv.hpp"
#include <chrono>
#include <matrix.hpp>

using namespace std;
using namespace chrono;

void writeHeader(ostream& csv)
{
    csv << "matrixKind, multMethod, threadCount, dimension, sparsity, time\n";
}

void writeResult(ostream& csv, const std::string& matrixKind, const std::string& multMethod, unsigned threadCount,
                 unsigned dimension, float sparsity, microseconds time)
{
    csv << matrixKind << ',' << multMethod << ',' << threadCount << ',' << dimension << ',' << sparsity << ','
        << time.count() << endl;
}