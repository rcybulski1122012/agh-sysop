all:
	make compile_1024
	make compile_1

compile_1024:
	gcc -Wall -o main_1024 main.c

compile_1:
	gcc -Wall -o main_1 main.c -D MY_BLOCK_SIZE=2

clean:
	rm -f *.txt main_1024 main_1 tmp1 tmp2

test:
	make compile_1024
	make compile_1
	echo "\nabcdefghijklmnoprstuvwxyz" > stest_file.txt
	echo "\nzyxwvutsrponmlkjihgfedcba" > expected.txt

	head -c 10K < /dev/urandom > btest_file.txt

	./main_1024 btest_file.txt tmp1.txt;
	./main_1024 tmp1.txt tmp2.txt;
	diff -s tmp2.txt btest_file.txt

	./main_1 stest_file.txt s_result_1.txt
	diff -s expected.txt s_result_1.txt

	./main_1024 stest_file.txt s_result_1024.txt
	diff -s expected.txt s_result_1024.txt

	echo "Po 1:" > pomiar_zad_2.txt
	./main_1 btest_file.txt main_1_result.txt >> pomiar_zad_2.txt

	echo "Po 1024" >> pomiar_zad_2.txt
	./main_1024 btest_file.txt main_1024_result.txt >> pomiar_zad_2.txt

	rm -f tmp1.txt tmp2.txt main_1_result.txt main_1024_result.txt main_1 main_1024 btest_file.txt stest_file.txt s_result_1.txt s_result_1024.txt expected.txt
