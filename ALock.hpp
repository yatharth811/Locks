#pragma once
#include <atomic>
#include <vector>
#include "Lock.hpp"

class ALock : public Lock {
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
