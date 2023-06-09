
#include "csv.hpp"
#include "helpers.hpp"
#include "matrix.hpp"
#include "mult.hpp"
#include "omp.h"

using namespace std;

template <unsigned DIM> void benchmarkAll(ostream &csvFile, int threadCount)
{
    omp_set_num_threads(threadCount);

    auto highLeft = randomMat<DIM>(highChance);
    auto highRight = randomMat<DIM>(highChance);
    auto middleLeft = randomMat<DIM>(middleChance);
    auto middleRight = randomMat<DIM>(middleChance);
    auto lowLeft = randomMat<DIM>(lowChance);
    auto lowRight = randomMat<DIM>(lowChance);

    auto benchmarkOne = [&](const string &name, matrix<DIM> (*func)(const matrix<DIM> &, const matrix<DIM> &)) {
        writeResult(csvFile, name, threadCount, DIM, highChance / 100.f, measure(func, highLeft, highRight));
        writeResult(csvFile, name, threadCount, DIM, middleChance / 100.f, measure(func, middleLeft, middleRight));
        writeResult(csvFile, name, threadCount, DIM, lowChance / 100.f, measure(func, lowLeft, lowRight));
    };

    benchmarkOne("sparse naive sequential", multSparseSeqNaive<DIM>);
    benchmarkOne("dense naive sequential", multDenseSeqNaive<DIM>);
    benchmarkOne("sparse mutex OpenMP", multSparseOmpNaiveMutex<DIM>);
    benchmarkOne("dense mutex OpenMP", multDenseOmpNaiveMutex<DIM>);
    benchmarkOne("sparse conditional-critical OpenMP", multSparseOmpNaiveConditionalCritical<DIM>);
    benchmarkOne("sparse reduced-critical OpenMP", multSparseOmpNaiveReducedCritical<DIM>);
}

int main()
{
    if (!testMatGetSet())
        cout << "Matrix class does not fulfill get/set contract." << endl;
    else
        cout << "Matrix class fulfills contract." << endl;

    if (!testMultMethod(multSparseSeqNaive<50>))
        cout << "Method1 is incorrect." << endl;
    if (!testMultMethod(multDenseSeqNaive<50>))
        cout << "Method2 is incorrect." << endl;
    if (!testMultMethod(multSparseOmpNaiveMutex<50>))
        cout << "Method3 is incorrect." << endl;
    if (!testMultMethod(multDenseOmpNaiveMutex<50>))
        cout << "Method4 is incorrect." << endl;
    if (!testMultMethod(multSparseOmpNaiveConditionalCritical<50>))
        cout << "Method5 is incorrect." << endl;
    if (!testMultMethod(multSparseOmpNaiveReducedCritical<50>))
        cout << "Method6 is incorrect." << endl;

    const std::string csvName = "results.csv";
    ofstream csvFile;
    csvFile.open(csvName);

    writeHeader(csvFile);

    cout << "Benchmarking..." << endl;

    for (int i = 2; i < 17; i++)
    {
        cout << "Iteration " << i << " finished." << endl;
        benchmarkAll<50>(csvFile, i);
        benchmarkAll<100>(csvFile, i);
        benchmarkAll<150>(csvFile, i);
    }

    cout << "Written results to " << csvName << "\n";
    return 0;
}
