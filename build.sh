#!/bin/bash

CC="g++"
ROOT=$(dirname $0)
SRC=$(find $ROOT/src -name *.cpp)
CXXFLAGS="-Wall -Wpedantic -std=c++20 -Wextra -g -fsanitize=address" # -DDEBUG # uncomment when wanting to see some debug info
# CXXFLAGS="-Wall -Wpedantic -Werror -std=c++20 -Wextra -g -fsanitize=address -O3 -pipe -march=native"

$CC $CXXFLAGS $SRC -o $ROOT/bin/project
