#include <iostream>
#include <vector>
#include <atomic>
#include <thread>


class QNode {
  public: 
  QNode() : locked(false) , next(nullptr) {}
  bool locked;
  QNode* next;
};

class MCSLock {
  std::atomic<QNode*> tail;
  thread_local static QNode* myNode;

  public:
  MCSLock() : tail(nullptr) {}

  void lock() {
    QNode* pred = tail.exchange(myNode);
    if (pred != nullptr) {
      myNode->locked = true;
      pred->next = myNode;

      while (myNode->locked) {}
    }
  }

  void unlock() {
    if (myNode->next == nullptr) {
      if (tail.compare_exchange_strong(myNode, nullptr, std::memory_order_acq_rel)) 
        return;
      
      while (myNode->next == nullptr) {}
    }

    myNode->next->locked = false;
    myNode->next = nullptr;
  }
};

thread_local QNode* MCSLock::myNode = new QNode();
MCSLock mcs_lock;


int counter{};

void compute(void) {
  mcs_lock.lock();
  // std::cout << "I entered" << std::endl;
  for (int i = 1; i <= 10000; i += 1) {
    counter += 1;
  }
  mcs_lock.unlock();
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