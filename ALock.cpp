#include <iostream>
#include <vector>
#include <atomic>
#include <thread>

class ALock {
  thread_local static int myslot;
  std::atomic<int> tail {0};
  std::vector<bool> flag;
  int size;

  public:
    ALock(int capacity) : size(capacity) {
      flag.assign(size, false);
      flag[0] = true;
    }

    void lock() {
      int slot = (tail++) % size;
      myslot = slot;
      while (!flag[slot]);
    }

    void unlock() {
      int slot = myslot;
      flag[slot] = false;
      flag[(slot + 1) % size] = true;
    }
};
thread_local int ALock::myslot = 0;
int counter{};

ALock alock(1000);

void compute(void) {
  alock.lock();
  for (int i = 1; i <= 10000; i += 1) {
    counter += 1;
  }
  alock.unlock();
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