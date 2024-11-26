#! /bin/bash

make clean && make -j$(nproc) && ./edge.out