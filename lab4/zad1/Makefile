all:
	make compile_fork
	make compile_exec

compile_fork: main.c
	gcc -Wall -o main_fork main.c

compile_exec: main.c test_exec.c
	gcc -Wall -o main_exec main.c -D USE_EXEC
	gcc -Wall -o test_exec test_exec.c

test:
	make compile_fork
	make compile_exec
	echo "Fork:\n" > results.txt
	echo "Ignore:\n" >> results.txt
	./main_fork ignore >> results.txt
	echo "\n\nHandle:\n" >> results.txt
	./main_fork handler >> results.txt
	echo "\n\nMask:\n" >> results.txt
	./main_fork mask >> results.txt
	echo "\n\nPending:\n" >> results.txt
	./main_fork pending >> results.txt
	echo "\n\n\nExec:\n" >> results.txt
	echo "Ignore:\n" >> results.txt
	./main_exec ignore >> results.txt
	echo "\n\nMask:\n" >> results.txt
	./main_exec mask >> results.txt
	echo "\n\nPending:\n" >> results.txt
	./main_exec pending >> results.txt
	echo "\n\nHandle:\n" >> results.txt
	./main_exec handler >> results.txt

clean:
	rm -f main_fork main_exec test_exec *.txt