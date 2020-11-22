#!/bin/bash
set -e

# g++ device.cc elf64.cc section64.cc program64.cc readelf.cc
g++ -g3 device.cc elf64.cc section64.cc program64.cc symbol.cc dynamic.cc readelf.cc asm.cc -o debug
#rm debug
