# Different Locks Implementation

We have implemented following locks:

1. Test-And-Set Lock (TAS)
2. Test-And-Test-And-Set Lock (TTAS)
3. Anderson Lock (ALock)
4. CLH Lock
5. MCS Lock

### How to Run?
`g++ -std=c++20 -Wall main.cpp -o main`
`./main [Number of Threads]`


### Average Times
To get average runtimes, run  `sh run.sh [number of threads] [number of iterations]`. Average runtimes are printed in the order given above.
