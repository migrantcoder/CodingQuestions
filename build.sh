#!/bin/bash

# Run from project root.

BUILDDIR=build
SRCDIR=src

if [[ ! -d $BUILDDIR ]]
then
    mkdir $BUILDDIR
fi

for subdir in `ls ${SRCDIR}`
do
    pushd ${SRCDIR}/$subdir > /dev/null
    for srcfile in `ls *.cpp`
    do
        srcfile_basename=$(basename ${srcfile} .cpp)
        output_name="${subdir}-${srcfile_basename}"
        echo -n "building ${srcfile} to ${output_name} ... "
        clang++ \
            -ggdb3 -Wall -Wpedantic -O1 --std=c++1y \
            -I../../${SRCDIR} \
            -o ../../${BUILDDIR}/${output_name} \
            ${srcfile}
        rc=$?
        if [ "$rc" -ne "0" ]
        then
            popd > /dev/null
            exit $rc
        fi
        echo "done"
    done
    popd > /dev/null
done

exit 0
