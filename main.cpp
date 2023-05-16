// source:
// https://www.ibm.com/docs/en/xcfbg/121.141?topic=openmp-running-complete-program

#include <iostream>
#include <omp.h>
#include <string>

constexpr int screen_width = 80;
constexpr int table_size = screen_width * 20;

int main() {
  int i, j;
  int prime[table_size + 1];

#pragma omp parallel
  {
    // notice that chaining multiple operations with the same
    std::cout << "Greetings from thread " +
                     std::to_string(omp_get_thread_num()) + ".\n";
  }

/* Also try schedule(dynamic, SCREEN_WIDTH) for more random output */
#pragma omp parallel for shared(prime) private(i, j)
  for (i = 1; i <= table_size; i++) {
    prime[i] = 1; /* assume i is prime until proven otherwise */
    for (j = 2; j < i; j++) {
      if (i % j == 0) {
        prime[i] = 0; /* j divides i, so i is not prime */
        break;
      }
    }
  }

  std::cout << "Table of primes, marked with '*':" << std::endl;
  for (i = 1; i <= table_size; i++) {
    std::cout << (prime[i] ? '*' : '-');
    if (i % screen_width == 0)
      std::cout << std::endl;
  }

  return 0;
}
