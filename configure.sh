#!/bin/bash

sudo apt-get install python3 python3-dev libsfml-dev cmake make
mkdir build && cd build && cmake .. && make
cp Unamed ../
