#include <iostream>
#include <vector>
#include <atomic>
#include <thread>

std::atomic<bool> locked = false;
int counter{};

void lock() {
  while (locked.exchange(true));  
}

void unlock() {
  locked.store(false);
}

void compute(void) {

  lock();
  for (int i = 1; i <= 10000; i += 1) {
    counter += 1;
  }
  unlock();
}

int main() {

  std::vector<std::thread> threads;

  for (int i = 1; i <= 1000; i += 1) {
    threads.emplace_back(compute);
  }

  for (int i = 1; i <= 1000; i += 1) {
    if (threads[i - 1].joinable())
      threads[i - 1].join();
  }

  std::cout << counter << std::endl;


  return 0;
}