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
    clang++ -ggdb3 -O0 --std=c++1y -I${SRCDIR} -Wall -Wpedantic -o ${BUILDDIR}/${d}-example $(find ${SRCDIR}/${d} -name *.cpp)
    let rc=$?
    if [ "$rc" -ne "0" ]
    then
        exit $rc
    fi
done

exit 0
