#!/bin/bash
set -e
# g++ device.cc elf.cc readelf.cc
g++ device.cc elf64.cc section64.cc program64.cc readelf.cc
./a.out
rm a.out
