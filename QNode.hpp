#pragma once
#include <memory>
// #include "CLHLock.hpp"
// #include "MCSLock.hpp"

class QNode {
  bool locked;
  std::shared_ptr<QNode> next;
  friend class CLHLock;
  friend class MCSLock;

  public: 
  QNode() : locked(false) , next(nullptr) {}
};
