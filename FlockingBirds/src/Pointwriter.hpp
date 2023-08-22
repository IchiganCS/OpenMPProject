
#ifndef POINTWRITER_HPP
#define POINTWRITER_HPP
#include <chrono>
#include <fstream>
#include <iostream>

class Time {
private:
  std::chrono::high_resolution_clock::time_point s, e;

public:
  void start() { s = std::chrono::high_resolution_clock::now(); }
  void end() { e = std::chrono::high_resolution_clock::now(); }
  double getDurationinNanos() {
    return std::chrono::duration_cast<std::chrono::nanoseconds>(e - s).count();
  }
  void logDuration(const std::string &opName) {
    double ms = 0;
    ms = std::chrono::duration_cast<std::chrono::milliseconds>(e - s).count();
    printf("%s took %lf ms\n", opName.c_str(), ms);
  }
};

class PointWriter {
private:
  std::string filename;
  std::ofstream file;

public:
  PointWriter(const std::string &filename) : filename(filename) {
    openFile();
    file << "Num Of Birds"
         << ","
         << "Num Of Threads"
         << ","
         << "Num Of Obstacles"
         << ","
         << "Time for Seq"
         << ","
         << "Time For Parallel"
         << ","
         << "Time For Parallel with Reduction"
         << ","
         << "Time for Parallel with offloading"
         << "\n";
  }
  PointWriter(const PointWriter &other) {
    filename = other.filename;
    file.open(filename);
  }
  ~PointWriter() { closeFile(); }
  bool openFile() {
    file.open(filename);
    if (!file) {
      std::cerr << "Failed to open file: " << filename << std::endl;
      return false;
    }
    return true;
  }

  void closeFile() {
    file.close();
    std::cout << "Points written to file: " << filename << std::endl;
  }

  void writePoint(int NumOfBirds, int NumOfThreads, int NumOfObstacles,
                  double TimeForSeq, double TimeForParallel,
                  double TimeForParallelWithReduction,
                  double timeforparallelWithOffLodaing) {
    if (!file.is_open()) {
      std::cerr << "File is not open!" << std::endl;
      return;
    }

    file << NumOfBirds << "," << NumOfThreads << "," << NumOfObstacles << ","
         << TimeForSeq << "," << TimeForParallel << ","
         << TimeForParallelWithReduction << "," << timeforparallelWithOffLodaing
         << "\n";
    file.flush();
  }
};
#endif