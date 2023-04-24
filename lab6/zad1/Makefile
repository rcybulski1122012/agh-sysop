run_client:
	make compile_client
	./client

run_server:
	make compile_server
	./server

compile_client: client.c common.h
	gcc -Wall -o client client.c

compile_server: server.c common.h
	gcc -Wall -o server server.c

clean:
	rm -f client server logs