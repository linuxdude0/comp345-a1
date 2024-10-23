#!/bin/bash

ROOT=$(dirname $0)
SRC=$(find $ROOT/src -name *.cpp)
# CXXFLAGS="-Wall -Wpedantic -std=c++20 -Wextra -g -fsanitize=address -DDEBUG" # uncomment when wanting to see some debug info
CXXFLAGS="-Wall -Wpedantic -Werror -std=c++20 -Wextra -g -fsanitize=address -O3"

g++ $CXXFLAGS $SRC -o $ROOT/bin/project
