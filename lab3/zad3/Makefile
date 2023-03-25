all:
	make compile

compile: main.c
	gcc -Wall -o main main.c

test:
	make compile
	./main ../.. "#include"
	make clean

clean:
	rm -f main