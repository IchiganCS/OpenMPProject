#include <chrono>
#include <fstream>

void writeHeader(std::ostream &csvFile);

void writeResult(std::ostream &csvFile, const std::string& matrixKind, const std::string &multMethod, unsigned threadCount, unsigned dimension,
                 float sparsity, std::chrono::microseconds time);
