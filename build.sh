#!/bin/bash
set -e
if [ $# -ne 1 ]; then
	echo -e "Please type option:\n"
	echo -e "  -h : Elf header"
	echo -e "  -S : Section header"
	echo -e "  -p : Program header"
	echo -e "  -s : Symbol table"
	exit 1
fi

# g++ device.cc elf64.cc section64.cc program64.cc readelf.cc
g++ device.cc elf64.cc section64.cc program64.cc symbol.cc readelf.cc
./a.out $1
rm a.out
