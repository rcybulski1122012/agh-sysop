all:
	make compile

compile: main.c
	gcc -Wall -o main main.c

test:
	make compile
	./main ~
	mkdir temp
	./main ./temp
	rmdir temp
	./main /bin
	make clean

clean:
	rm -f main