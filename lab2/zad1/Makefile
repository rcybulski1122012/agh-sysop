all:
	make compile
	make compile_sys_funcs

compile:
	gcc -Wall -o main main.c

compile_sys_funcs:
	gcc -Wall -o main_sys_funcs main.c -D USE_SYS_FUNC

clean:
	rm -f main main_sys_funcs *.txt

test:
	make compile
	make compile_sys_funcs
	head -c 100K < /dev/urandom > test_file.txt
	echo "aaaabbababababbababababababbababababababa" >> test_file.txt
	tr b a < test_file.txt > tr_result.txt;
	echo "Funckje C" > pomiar_zad_1.txt
	./main b a test_file.txt result_main.txt >> pomiar_zad_1.txt
	diff -s tr_result.txt result_main.txt
	echo "Funckje systemowe" >> pomiar_zad_1.txt
	./main_sys_funcs b a test_file.txt result_main_sys_funcs.txt >> pomiar_zad_1.txt
	diff -s tr_result.txt result_main_sys_funcs.txt
	rm -f test_file.txt tr_result.txt result_main.txt result_main_sys_funcs.txt
	rm -f main main_sys_funcs


