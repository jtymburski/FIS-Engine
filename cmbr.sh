#!/bin/bash

make clean
rm -f Makefile
qmake Univursa.pro
make
./bin/Univursa
