#include <iostream>
#include <vector>
#include <atomic>
#include <thread>


class QNode {
  public: 
  QNode() : locked(false) {}
  bool locked;
};

class CLHLock {
  std::atomic<QNode*> tail;
  thread_local static QNode* myPred;
  thread_local static QNode* myNode;

  public:
  CLHLock() {
    tail = new QNode();
  };

  void lock() {
    myNode->locked = true;
    QNode* pred = tail.exchange(myNode);
    myPred = pred;
    while (pred->locked) {}
  }

  void unlock() {
    myNode->locked = false;
    myNode = myPred;
  }
};

thread_local QNode* CLHLock::myPred = nullptr;
thread_local QNode* CLHLock::myNode = new QNode();
CLHLock clh_lock;


int counter{};

void compute(void) {
  clh_lock.lock();
  for (int i = 1; i <= 10000; i += 1) {
    counter += 1;
  }
  clh_lock.unlock();
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