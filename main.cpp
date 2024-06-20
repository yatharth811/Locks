#include <iostream>
#include <iomanip>
#include <vector>
#include <atomic>
#include <thread>
#include <chrono>
#include <string>
#include "TTAS.hpp"
#include "TAS.hpp"
#include "ALock.hpp"
#include "CLHLock.hpp"
#include "MCSLock.hpp"

int counter{};
CLHLock lock;

template<typename T>
void computeCRTP(Lock<T> &lock, int limit) {
  lock.lock();
  for (int i = 1; i <= limit; i += 1) {
    counter += 1;
  }
  lock.unlock();
}

template<typename T>
void compute(Lock<T> &lock, int limit, int nthreads, std::string &&x) {
  auto start = std::chrono::high_resolution_clock::now();

  std::vector<std::thread> threads;
  for (int i = 0; i < nthreads; i += 1) {
    threads.emplace_back(computeCRTP<T>, std::ref(lock), limit);
  }

  for (int i = 0; i < nthreads; i += 1) {
    if (threads[i].joinable())
      threads[i].join();
  }

  auto end = std::chrono::high_resolution_clock::now();
  double time_taken = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count() * 1e-9;


  std::cout << "\e[1;33m \u26BF \e[0m";
  std::cout << " Lock: " << x << std::endl;
  std::cout << " Expected Value of Counter: " << limit * nthreads << std::endl;
  std::cout << " Received Value: " << counter << std::endl;
  std::cout << (counter == limit * nthreads ? " AC \e[1;32m \u2714 \e[0m" : " WA \e[1;31m \u00D7 \e[0m") << std::endl;
  std::cout << " Time taken: " << std::fixed << std::setprecision(10) << time_taken << std::endl;
  std::cout << std::endl;
}


int main(int argc, char** argv) {

  int nthreads = std::stoi(argv[1]);
  TAS tas;
  TTAS ttas;
  ALock aLock(nthreads);
  CLHLock clhLock;
  MCSLock mcsLock;

  counter = 0;
  compute(tas, 1'000'000 / nthreads, nthreads, "TAS");
  
  counter = 0;
  compute(ttas, 1'000'000 / nthreads, nthreads, "TTAS");
    
  counter = 0;
  compute(clhLock, 1'000'000 / nthreads, nthreads, "CLHLock");

  counter = 0;
  compute(mcsLock, 1'000'000 / nthreads, nthreads, "MCSLock");

  counter = 0;
  compute(aLock, 1'000'000 / nthreads, nthreads, "ALock");

  return 0;
}
