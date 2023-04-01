all:
	make compile


compile: test_siginfo.c test_nodefer.c test_resethand.c
	gcc -Wall -o siginfo test_siginfo.c
	gcc -Wall -o nodefer test_nodefer.c
	gcc -Wall -o resethand test_resethand.c


test:
	make compile

	echo "test SA_SIGINFO\n" > results.txt
	./siginfo >> results.txt
	echo "\n\ntest SA_NODEFER\n" >> results.txt
	./nodefer >> results.txt
	echo "\n\ntest SA_RESETHAND\n" >> results.txt
	./resethand >> results.txt


clean:
	rm -f siginfo nodefer resethand *.txt