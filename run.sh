#!/bin/bash
nthreads=$1
declare -A avg_times
for ((i=0; i<5; i++)); do
  avg_times[$i]=0;
done

lim=$2
for ((i=1; i<=$lim; i++)); do
  times=($(./main.out $nthreads | grep -o "Time taken: [0-9.]*" | awk '{print $3}'))
  for ((j=0; j<5; j++)); do
    # echo ${times[j]}
    avg_times[$j]=$(echo "${avg_times[$j]} + ${times[$j]}" | bc)
  done
done

echo "Average times are:"
for ((i=0; i<5; i++)); do
  echo $(echo "scale=10; ${avg_times[$i]} / ${lim}" | bc)
done
