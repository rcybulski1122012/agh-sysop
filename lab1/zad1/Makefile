NAME=libwc

all:
	make static
	make dynamic

static:
	gcc -Wall -c $(NAME).c
	ar cr $(NAME).a $(NAME).o

dynamic:
	gcc -Wall -fPIC -c $(NAME).c
	gcc -Wall -fPIC -shared -o $(NAME).so $(NAME).o

clean:
	rm -f *.o *.a *.so
	rm -f main