NAME=libwc

static_lib:
	gcc -Wall -c $(NAME).c
	ar cr $(NAME).a $(NAME).o

dynamic_lib:
	gcc -Wall -fPIC -c $(NAME).c
	gcc -Wall -fPIC -shared -o $(NAME).so $(NAME).o

clean:
	rm -f *.o *.a *.so ./main_static ./main_dynamic ./main_dll *.txt

static_main:
	gcc -Wall main.c -o main_static -L. $(NAME).a

dynamic_main:
	gcc -Wall main.c -o main_dynamic -L. -lwc

dll_main:
	gcc -Wall main.c -D USE_DLL -o main_dll

_create_test_file:
	echo "init 100" > test_commands.txt
	echo "count ./dll_loader.h" >> test_commands.txt
	echo "count ./libwc.h" >> test_commands.txt
	echo "count ./libwc.c" >> test_commands.txt
	echo "count ./main.c" >> test_commands.txt
	echo "count ./Makefile" >> test_commands.txt
	echo "show 0" >> test_commands.txt
	echo "show 1" >> test_commands.txt
	echo "show 2" >> test_commands.txt
	echo "show 3" >> test_commands.txt
	echo "show 4" >> test_commands.txt
	echo "delete 0" >> test_commands.txt
	echo "delete 1" >> test_commands.txt
	echo "delete 2" >> test_commands.txt
	echo "delete 3" >> test_commands.txt
	echo "delete 4" >> test_commands.txt
	echo "destroy" >> test_commands.txt
	echo "quit" >> test_commands.txt


test_static:
	make _create_test_file
	make static_lib
	make static_main
	./main_static < test_commands.txt > results_static.txt
	rm -f test_commands.txt



test_dynamic:
	make _create_test_file
	make dynamic_lib
	make dynamic_main
	./main_dynamic < test_commands.txt > results_shared.txt
	rm -f test_commands.txt


test_dll:
	make _create_test_file
	make dynamic_lib
	make dll_main
	./main_dll < test_commands.txt > results_dynamic.txt
	rm -f test_commands.txt