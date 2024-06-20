#pragma once
#include <atomic>
#include "Lock.hpp"

class TAS : public Lock<TAS> {
  std::atomic<bool> locked = false;
  
  public:
  void lock() {
    while (locked.exchange(true));  
  }

  void unlock() {
    locked.store(false);
  }
};
