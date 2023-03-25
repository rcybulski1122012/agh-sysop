all:
	make compile

compile: main.c
	gcc -Wall -o main main.c

test:
	make compile
	./main 50
	make clean

clean:
	rm -f main