all:
	make compile

compile:
	gcc -Wall -o main main.c

clean:
	rm -f main

test:
	make compile
	./main .
	make clean