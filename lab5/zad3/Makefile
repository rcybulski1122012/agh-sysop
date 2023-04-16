all:
	make compile

test:
	make compile

	./main 0.00000001 1 > result.txt
	./main 0.00000001 10 >> result.txt
	./main 0.00000001 100 >> result.txt

	./main 0.000000001 1 >> result.txt
	./main 0.000000001 10 >> result.txt
	./main 0.000000001 100 >> result.txt

	./main 0.0000000001 1 >> result.txt
	./main 0.0000000001 10 >> result.txt
	./main 0.0000000001 100 >> result.txt

compile: main.c integral.c
	gcc -Wall -o main main.c
	gcc -Wall -o integral integral.c

clean:
	rm -f main *.txt integral .fifo fifo
