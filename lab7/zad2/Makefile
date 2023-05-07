all:
	make compile_all

test:
	make compile_all
	echo "\nTesting for 7 hairdressers, 5 chairs, 3 places in the waiting room and 30 clients\n"
	./main 7 5 3 30

compile_all:
	make compile_main
	make compile_client
	make compile_hairdresser

compile_main: main.c sem.c shm.c
	gcc -Wall main.c shm.c sem.c -o main -lrt -lpthread

compile_client: client.c sem.c shm.c
	gcc -Wall client.c shm.c sem.c -o client -lrt -lpthread

compile_hairdresser: hairdresser.c sem.c shm.c
	gcc -Wall hairdresser.c shm.c sem.c -o hairdresser -lrt -lpthread

clean:
	rm -f main hairdresser client