#! /bin/bash

num=$(($(nproc) - 1))
num=$(($num > 0 ? $num : 1))

make clean && make -j$num && ./edge.out