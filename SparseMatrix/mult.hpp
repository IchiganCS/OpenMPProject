#include "matrix.hpp"
#include <mutex>

/// Preliminary summary:
/// - Not every parallelization makes it faster.
/// - Mutexes are worse than #omp critical (at least for OpenMP)
/// - Caching is useful (compare reduced-critical, conditional-critical)
/// - Many threads can lead to congestion: 2 threads are faster than 16.
/// - Compiler optimization benefits sequential programs more than parallel programs.
/// - Overall: The fewer synchronization points a program has, the faster it may perform.


/// This method uses the fact that the matrices are sparse and always checks if an entry is zero before
/// fetching a potentially unrequired value and only writes values if they are non-zero. It does not parallelize its
/// operations.
template <unsigned DIM> matrix<DIM> multSparseSeqNaive(const matrix<DIM> &left, const matrix<DIM> &right)
{
    matrix<DIM> result;

    for (int r = 0; r < DIM; r++)
    {
        for (int c = 0; c < DIM; c++)
        {
            int val = 0;

            for (int i = 0; i < DIM; i++)
            {
                int thisVal = left.get(r, i);
                if (thisVal != 0)
                    val += thisVal * right.get(i, c);
            }

            if (val != 0)
                result.set(r, c, val);
        }
    }

    return result;
}

/// This method does not consider that the matrix is sparse and just reads and writes values
/// as if a dense matrix is expected. It does not parallelize its operations.
template <unsigned DIM> matrix<DIM> multDenseSeqNaive(const matrix<DIM> &left, const matrix<DIM> &right)
{
    matrix<DIM> result;

    for (int r = 0; r < DIM; r++)
    {
        for (int c = 0; c < DIM; c++)
        {
            int val = 0;

            for (int i = 0; i < DIM; i++)
            {
                val += left.get(r, i) * right.get(i, c);
            }

            result.set(r, c, val);
        }
    }

    return result;
}

/// This method works quite like its sparse sequential counterpart, but is a little bit more optimized.
/// It uses mutexes to ensure thread safety and that mutex is only acquired if the written value is non-zero.
/// This has lead to a significant performance improvement.
template <unsigned DIM> matrix<DIM> multSparseOmpNaiveMutex(const matrix<DIM> &left, const matrix<DIM> &right)
{
    matrix<DIM> result;
    std::mutex resultMutex;

#pragma omp parallel for
    for (int r = 0; r < DIM; r++)
    {
        for (int c = 0; c < DIM; c++)
        {
            int val = 0;

            for (int i = 0; i < DIM; i++)
            {
                int leftVal = left.get(r, i);
                if (leftVal != 0)
                    val += left.get(r, i) * right.get(i, c);
            }

            if (val != 0)
            {
                const auto lock = std::lock_guard(resultMutex);
                result.set(r, c, val);
            }
        }
    }

    return result;
}

/// This method does not take into consideration that the matrices are sparse. It multiplies them just like
/// they were sparse matrices. It has the same common mutex optimization for locking only if the value is non-zero.
template <unsigned DIM> matrix<DIM> multDenseOmpNaiveMutex(const matrix<DIM> &left, const matrix<DIM> &right)
{
    matrix<DIM> result;
    std::mutex resultMutex;

#pragma omp parallel for
    for (int r = 0; r < DIM; r++)
    {
        for (int c = 0; c < DIM; c++)
        {
            int val = 0;

            for (int i = 0; i < DIM; i++)
            {
                val += left.get(r, i) * right.get(i, c);
            }

            if (val != 0)
            {
                const auto lock = std::lock_guard(resultMutex);
                result.set(r, c, val);
            }
        }
    }

    return result;
}


/// This method uses the #omp critical construct to ensure thread safety. A critical section is only entered, if 
/// the value to be written is non-zero.
template <unsigned DIM>
matrix<DIM> multSparseOmpNaiveConditionalCritical(const matrix<DIM> &left, const matrix<DIM> &right)
{
    matrix<DIM> result;
    std::mutex resultMutex;

#pragma omp parallel for
    for (int r = 0; r < DIM; r++)
    {
        for (int c = 0; c < DIM; c++)
        {
            int val = 0;

            for (int i = 0; i < DIM; i++)
            {
                int leftVal = left.get(r, i);
                if (leftVal != 0)
                    val += left.get(r, i) * right.get(i, c);
            }

            if (val != 0)
            {
#pragma omp critical
                result.set(r, c, val);
            }
        }
    }

    return result;
}


/// This method caches changes and flushes the entire cache in a single critical block. This reduces the 
/// number of overall entered critical blocks and is the fastest method.
template <unsigned DIM> matrix<DIM> multSparseOmpNaiveReducedCritical(const matrix<DIM> &left, const matrix<DIM> &right)
{
    matrix<DIM> result;

#pragma omp parallel for
    for (int r = 0; r < DIM; r++)
    {
        std::vector<std::tuple<unsigned, unsigned, int>> cache;
        // this just a guess for a good value
        // it fits probably all results for a sparse matrix
        // and needs only one resizing to fit all values for a dense matrix
        cache.reserve(DIM / 2); 

        for (int c = 0; c < DIM; c++)
        {
            int val = 0;

            for (int i = 0; i < DIM; i++)
            {
                int leftVal = left.get(r, i);
                if (leftVal != 0)
                    val += left.get(r, i) * right.get(i, c);
            }

            if (val != 0)
            {
                cache.push_back(std::tuple(r, c, val));
            }
        }

        if (!cache.empty())
        {
#pragma omp critical
            for (auto &[r, c, val] : cache)
                result.set(r, c, val);
        }
    }

    return result;
}