NAME=libwc

static_lib:
	gcc -Wall -c $(NAME).c
	ar cr $(NAME).a $(NAME).o

dynamic_lib:
	gcc -Wall -fPIC -c $(NAME).c
	gcc -Wall -fPIC -shared -o $(NAME).so $(NAME).o

clean:
	rm -f *.o *.a *.so ./main_static ./main_dynamic ./main_dll

static_main:
	gcc -Wall main.c -o main_static -L. $(NAME).a

dynamic_main:
	gcc -Wall main.c -o main_dynamic -L. -lwc

dll_main:
	gcc -Wall main.c -D USE_DLL -o main_dll

