gcc -g -c seq.c -o seq.o
gcc -g -c array.c -o array.o
gcc -g -c list.c -o list.o
gcc -g -c test.c -o test.o
gcc -g -c generic.c -o generic.o
gcc *.o -o test
