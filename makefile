list : list.o testListHelpers.o
	gcc -w -g list.o testListHelpers.o -o list
list.o : list.c testListHelpers.o
	gcc -c -w -g list.c -o list.o
testListHelpers.o : testListHelpers.c
	gcc -c -w -g testListHelpers.c -o testListHelpers.o
clear :
	rm list list.o testListHelpers.o