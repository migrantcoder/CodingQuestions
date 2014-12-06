#!/bin/bash

# Run from project root.

BUILDDIR=build
SRCDIR=src

if [[ ! -d $BUILDDIR ]]
then
    mkdir $BUILDDIR
fi

clang++ -ggdb3 -O2 --std=c++1y -I${SRCDIR} -Wall -Wpedantic -o ${BUILDDIR}/example $(find $SRCDIR -name *.cpp)
