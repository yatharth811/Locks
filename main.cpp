#include <iostream>
#include <iomanip>
#include <vector>
#include <atomic>
#include <thread>
#include <chrono>
#include "TTAS.hpp"
#include "TAS.hpp"
#include "ALock.hpp"
#include "CLHLock.hpp"
#include "MCSLock.hpp"

int counter{};
CLHLock lock;

void compute(Lock* lock, int limit) {
  lock->lock();
  for (int i = 1; i <= limit; i += 1) {
    counter += 1;
  }
  lock->unlock();
}

int main(int argc, char** argv) {

  int nthreads = std::stoi(argv[1]);

  std::vector<std::pair<std::string, Lock*>> locks;
  locks.emplace_back("TAS", new TAS());
  locks.emplace_back("TTAS", new TTAS());
  locks.emplace_back("ALock", new ALock(nthreads));
  locks.emplace_back("CLHLock", new CLHLock());
  locks.emplace_back("MCSLock", new MCSLock());
  
  for (auto [x, y]: locks) {
    counter = 0;
    
    auto start = std::chrono::high_resolution_clock::now();

    std::vector<std::thread> threads;
    for (int i = 0; i < nthreads; i += 1) {
      threads.emplace_back(compute, y, 1'000'000 / nthreads);
    }

    for (int i = 0; i < nthreads; i += 1) {
      if (threads[i].joinable())
        threads[i].join();
    }
    
    auto end = std::chrono::high_resolution_clock::now();
    double time_taken = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count() * 1e-9;

    std::cout << "\e[1;33m \u26BF \e[0m";
    std::cout << " Lock: " << x << std::endl;

    std::cout << " Expected Value of Counter: " << (1'000'000 / nthreads) * nthreads << std::endl;
    std::cout << " Received Value: " << counter << std::endl;
    std::cout << (counter == (1'000'000 / nthreads) * nthreads ? " AC \e[1;32m \u2714 \e[0m" : " WA \e[1;31m \u00D7 \e[0m") << std::endl;
    std::cout << " Time taken: " << std::fixed << std::setprecision(10) << time_taken << std::endl;
    std::cout << std::endl;
    
  }

  for (int i = 0; i < 5; i += 1) {
    delete locks[i].second;
  } 

  return 0;
}
