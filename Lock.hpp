#pragma once

template<typename T>
class Lock { 
  public:
  void lock() {
    static_cast<T*>(this)->lock();
  }

  void unlock() {
    static_cast<T*>(this)->unlock();
  }
  
  ~Lock() = default;
};
