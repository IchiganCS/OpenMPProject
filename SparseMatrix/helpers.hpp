#include "matrix.hpp"
#include <chrono>
#include <iostream>
#include <random>

template <unsigned DIM> std::ostream &operator<<(std::ostream &output, const matrix<DIM> &toWrite)
{
    constexpr int padWidth = 2;

    char oldFill = output.fill();
    output << std::setfill('0');

    for (int i = 0; i < DIM; i++)
    {
        for (int c = 0; c < DIM; c++)
        {
            output << toWrite.get(i, c);
            if (c < DIM - 1)
                output << ' ';
        }
        output << "\n";
    }

    output << std::setfill(oldFill);
    return output;
}

bool testMatGetSet();

/// This method executes a multiplication on a 50x50 example - if it matches the stored result, true is returned,
/// false otherwise.
bool testMultMethod(matrix<50> (*func)(const matrix<50>& left, const matrix<50>& right));

constexpr int highChance = 95;
constexpr int middleChance = 50;
constexpr int lowChance = 5;

template <unsigned DIM> matrix<DIM> randomMat(int chancePerHundred)
{
    std::random_device device;
    std::mt19937 twister(device());
    std::uniform_int_distribution<int> entryDistribution(1, 99);
    std::uniform_int_distribution<int> chanceDistribution(0, 100);

    matrix<DIM> result;
    for (int r = 0; r < DIM; r++)
    {
        for (int c = 0; c < DIM; c++)
        {
            if (chancePerHundred > chanceDistribution(twister))
                result.set(r, c, entryDistribution(twister));
        }
    }

    return result;
}

template <unsigned DIM>
std::chrono::microseconds measure(matrix<DIM> (*func)(const matrix<DIM> &, const matrix<DIM> &),
                                  const matrix<DIM> &left, const matrix<DIM> &right)
{
    auto start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < 5; i++)
        volatile auto _ = func(left, right); // disable discard
    auto end = std::chrono::high_resolution_clock::now();

    return duration_cast<std::chrono::microseconds>(end - start);
}