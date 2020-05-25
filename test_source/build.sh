#!/bin/zsh
set -e
g++ delete.cc
./a.out
rm a.out
