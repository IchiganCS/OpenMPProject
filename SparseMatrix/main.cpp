
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
        writeResult(csvFile, matrixName, name, threadCount, DIM, highChance / 100.f, measure(func, highLeft, highRight));
        cout << "Benchmarked high chance" << endl;
        writeResult(csvFile, matrixName, name, threadCount, DIM, middleChance / 100.f, measure(func, middleLeft, middleRight));
        cout << "Benchmarked middle chance" << endl;
        writeResult(csvFile, matrixName, name, threadCount, DIM, lowChance / 100.f, measure(func, lowLeft, lowRight));
        cout << "Benchmarked low chance" << endl;
    };

    benchmarkOne("sparse naive sequential", multSparseSeqNaive<DIM>);
    cout << "Benchmarked " << "sparse naive sequential" << endl;
    benchmarkOne("dense naive sequential", multDenseSeqNaive<DIM>);
    cout << "Benchmarked " << "dense naive sequential" << endl;
    benchmarkOne("sparse mutex OpenMP", multSparseOmpNaiveMutex<DIM>);
    cout << "Benchmarked " << "sparse mutex OpenMP" << endl;
    benchmarkOne("dense mutex OpenMP", multDenseOmpNaiveMutex<DIM>);
    cout << "Benchmarked " << "dense mutex OpenMP" << endl;
    benchmarkOne("sparse conditional-critical OpenMP", multSparseOmpNaiveConditionalCritical<DIM>);
    cout << "Benchmarked " << "sparse conditional-critical OpenMP" << endl;
    benchmarkOne("sparse reduced-critical OpenMP", multSparseOmpNaiveReducedCritical<DIM>);
    cout << "Benchmarked " << "sparse reduced-critical OpenMP" << endl;
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

    for (int i = 15; i < 25; i++)
    {
        benchmarkAll<50>(csvFile, i);
        cout << "Benchmarked dim 50." << endl;
        benchmarkAll<100>(csvFile, i);
        cout << "Benchmarked dim 100." << endl;
        benchmarkAll<150>(csvFile, i);
        cout << "Benchmarked dim 150." << endl;
        benchmarkAll<200>(csvFile, i);
        cout << "Benchmarked dim 200." << endl;
        cout << "Iteration " << i << " finished." << endl;
    }

    cout << "Written results to " << csvName << "\n";
    return 0;
}
