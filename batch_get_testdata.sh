#!/usr/bin/env bash

export PATH="app:bin:$PATH"

for pid in {1001..1005}
do
  echo "start get PID $pid"
  ./bin/tddump $pid
done
