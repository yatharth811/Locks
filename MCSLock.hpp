#pragma once
#include <atomic>
#include <memory>
#include "QNode.hpp"
#include "Lock.hpp"

class MCSLock : public Lock {
  std::atomic<std::shared_ptr<QNode>> tail;
  thread_local static std::shared_ptr<QNode> myNode;

  public:
  MCSLock() : tail(nullptr) {}

  void lock() {
    std::shared_ptr<QNode> pred = tail.exchange(myNode, std::memory_order_acq_rel);
    if (pred != nullptr) {
      myNode->locked = true;
      pred->next = myNode;

      while (myNode->locked) {}
    }
  }

  void unlock() {
    if (myNode->next == nullptr) {
      auto store = myNode;
      if (tail.compare_exchange_strong(myNode, nullptr, std::memory_order_acq_rel)) 
        return;

      myNode = store;
      while (myNode->next == nullptr) {}
    }

    myNode->next->locked = false;
    myNode->next = nullptr;
  }
};

thread_local std::shared_ptr<QNode> MCSLock::myNode(new QNode());
