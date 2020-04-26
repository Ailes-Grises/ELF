#!/bin/bash
set -e
# g++ device.cc elf.cc readelf.cc
g++ device.cc elf.cc section.cc readelf.cc
./a.out
rm a.out
