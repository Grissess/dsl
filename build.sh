#!/bin/sh
if [ -z "$CC" ]; then
    CC=cc
fi

$CC -g -c seq.c -o seq.o
$CC -g -c array.c -o array.o
$CC -g -c list.c -o list.o
$CC -g -c test.c -o test.o
$CC -g -c generic.c -o generic.o
$CC *.o -o test
