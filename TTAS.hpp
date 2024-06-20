#pragma once
#include <atomic>
#include "Lock.hpp"

class TTAS : public Lock<TTAS> {
  std::atomic<bool> locked = false;

  public:
  void lock() {
    while (true) {
      while (locked.load());

      if (!(locked.exchange(true))) {
        return;
      }
    }
  }

  void unlock() {
    locked.store(false);
  }
};
