#!/bin/bash

make clean
rm -f Makefile
rm -f bin/Univursa
qmake Univursa.pro
make
./bin/Univursa
