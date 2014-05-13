#!/bin/bash

mkdir -p ./testdata
mkdir -p ./testzone

export PATH=$PATH:./apps:./bin
sudo env PATH=$PATH miku
