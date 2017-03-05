#!/usr/bin/env bash

export PATH="app:bin:$PATH"

for pid in `eval echo "{$1..$2}"`
do
  echo "start get PID $pid"
  ./bin/tddump $pid
done
