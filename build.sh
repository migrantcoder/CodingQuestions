#!/bin/bash

# Run from project root.

BUILDDIR=build
SRCDIR=src

if [[ ! -d $BUILDDIR ]]
then
    mkdir $BUILDDIR
fi

for d in `ls ${SRCDIR}`
do
    clang++ -ggdb3 -O2 --std=c++1y -I${SRCDIR} -Wall -Wpedantic -o ${BUILDDIR}/${d}-example $(find ${SRCDIR}/${d} -name *.cpp)
done

