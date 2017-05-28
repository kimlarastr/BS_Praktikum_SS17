main: main.o function.o
	gcc -Wall -std=c99 -o main main.o function.o
main.o: main.c function.h
	gcc -Wall -std=c99 -c main.c
function.o: function.c function.h
	gcc -Wall -std=c99 -c function.c
clean:
	rm -f main main.o function.o
run: main
	./main
