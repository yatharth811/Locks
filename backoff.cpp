#include <iostream>
#include <vector>
#include <atomic>
#include <thread>
#include <chrono>
#include <random>

std::atomic<bool> locked = false;
int counter{};

class Backoff {
  public:
    int delay, maxDelay;
    Backoff(int min, int max) : delay(min), maxDelay(max) {}

    void backoff() {
      int sleeper = rand() % delay + 1;
      std::this_thread::sleep_for(std::chrono::milliseconds(sleeper));
      delay = delay << 1;
      delay = std::min(delay, maxDelay);
    }
};

void lock() {
  Backoff backoff(1, 1000);
  while (true) {
    while (locked.load());

    if (!(locked.exchange(true))) {
      return;
    }
    else {
      backoff.backoff();
    }
  }
}

void unlock() {
  locked.store(false);
}

void compute(void) {
  lock();
  for (int i = 1; i <= 100000; i += 1) {
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