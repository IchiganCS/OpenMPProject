
#include "helpers.hpp"
#include "matrix.hpp"
#include "mult.hpp"
#include "omp.h"

using namespace std;



int main()
{
    if (!testMatGetSet())
        cout << "Matrix class does not fulfill get/set contract" << endl;
    

    if (!testMultMethod(multSparseSeqNaive<50>))
        cout << "Method1 is incorrect" << endl;
    if (!testMultMethod(multDenseSeqNaive<50>))
        cout << "Method2 is incorrect" << endl;
    if (!testMultMethod(multSparseOmpNaiveMutex<50>))
        cout << "Method3 is incorrect" << endl;
    if (!testMultMethod(multDenseOmpNaiveMutex<50>))
        cout << "Method4 is incorrect" << endl;
    if (!testMultMethod(multSparseOmpNaiveConditionalCritical<50>))
        cout << "Method5 is incorrect" << endl;
    if (!testMultMethod(multSparseOmpNaiveReducedCritical<50>))
        cout << "Method6 is incorrect" << endl;


    omp_set_num_threads(2);

    constexpr unsigned DIM = 50;

    auto highLeft = randomMat<DIM>(highChance);
    auto highRight = randomMat<DIM>(highChance);
    auto middleLeft = randomMat<DIM>(middleChance);
    auto middleRight = randomMat<DIM>(middleChance);
    auto lowLeft = randomMat<DIM>(lowChance);
    auto lowRight = randomMat<DIM>(lowChance);

    benchmark(multSparseSeqNaive<DIM>, "sparse naive sequential", highLeft, highRight, middleLeft, middleRight, lowLeft,
              lowRight);
    benchmark(multDenseSeqNaive<DIM>, "dense naive sequential", highLeft, highRight, middleLeft, middleRight, lowLeft,
              lowRight);
    benchmark(multSparseOmpNaiveMutex<DIM>, "sparse mutex OpenMP", highLeft, highRight,
              middleLeft, middleRight, lowLeft, lowRight);
    benchmark(multDenseOmpNaiveMutex<DIM>, "dense mutex OpenMP", highLeft, highRight, middleLeft,
              middleRight, lowLeft, lowRight);
    benchmark(multSparseOmpNaiveConditionalCritical<DIM>, "sparse conditional-critical OpenMP", highLeft, highRight, middleLeft,
              middleRight, lowLeft, lowRight);
    benchmark(multSparseOmpNaiveReducedCritical<DIM>, "sparse reduced-critical OpenMP", highLeft, highRight, middleLeft,
              middleRight, lowLeft, lowRight);
    return 0;
}
