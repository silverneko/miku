#!/bin/bash

mkdir -p ./testdata
mkdir -p ./testzone
export PATH=$PATH:./app:./bin
miku --verbose --parallel 1 -b 10
