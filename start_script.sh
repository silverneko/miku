#!/bin/bash

mkdir -p ./testdata
mkdir -p ./testzone
sudo env PATH=$PATH:./apps:./bin miku --verbose
