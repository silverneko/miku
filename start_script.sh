#!/bin/bash

mkdir -p ./testdata
mkdir -p ./testzone
export PATH=$PATH:./app:./bin
miku --verbose --parallel 3 -b 10
