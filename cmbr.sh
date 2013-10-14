#!/bin/bash

make clean
rm -f Makefile
rm -f bin/Univursa
rm -f TEST.log

# Only recompile if parameter one is b(uild)
if [ -n "$1" ]
then
  if [ $1 = 'b' -o $1 = 'B' ] 
  then
    qmake5 Univursa.pro
    make
    ./bin/Univursa
  fi
fi
