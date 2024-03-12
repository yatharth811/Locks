#pragma once
#include <atomic>
#include <memory>
#include "QNode.hpp"
#include "Lock.hpp"

class CLHLock : public Lock {
  std::atomic<std::shared_ptr<QNode>> tail;
  thread_local static std::shared_ptr<QNode> myPred;
  thread_local static std::shared_ptr<QNode> myNode;

  public:
  CLHLock() {
    tail = std::shared_ptr<QNode>(new QNode());
  };

  void lock() {
    myNode->locked = true;
    std::shared_ptr<QNode> pred = tail.exchange(myNode);
    myPred = pred;
    while (pred->locked) {}
  }

  void unlock() {
    myNode->locked = false;
    myNode = myPred;
  }
};

thread_local std::shared_ptr<QNode> CLHLock::myPred(nullptr);
thread_local std::shared_ptr<QNode> CLHLock::myNode(new QNode());
