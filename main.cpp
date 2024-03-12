#include <iostream>
#include <vector>
#include <atomic>
#include <thread>
#include "TTAS.hpp"
#include "TAS.hpp"
#include "ALock.hpp"
#include "CLHLock.hpp"
#include "MCSLock.hpp"

int counter{};
CLHLock lock;

void compute(Lock* lock) {
  lock->lock();
  for (int i = 1; i <= 1000; i += 1) {
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

    std::vector<std::thread> threads;
    for (int i = 0; i < nthreads; i += 1) {
      threads.emplace_back(compute, y);
    }

    for (int i = 0; i < nthreads; i += 1) {
      if (threads[i].joinable())
        threads[i].join();
    }

    std::cout << "\e[1;33m \u26BF \e[0m";
    std::cout << " Lock: " << x << std::endl;

    std::cout << " Expected Value of Counter: " << nthreads * 1000 << std::endl;
    std::cout << " Received Value: " << counter << std::endl;
    std::cout << (counter == nthreads * 1000 ? " AC \e[1;32m \u2714 \e[0m" : " WA \e[1;32m \u00D7 \e[0m") << std::endl;
    std::cout << std::endl;
    
  }

  for (int i = 0; i < 5; i += 1) {
    delete locks[i].second;
  } 

  return 0;
}
