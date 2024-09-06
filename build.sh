#!/bin/bash

ROOT=$(dirname $0)
SRC=$(find $ROOT/src -name *.cpp)
CXXFLAGS="-Wall -Wextra -g -fsanitize=address"

g++ $CXXFLAGS $SRC -o $ROOT/bin/project
