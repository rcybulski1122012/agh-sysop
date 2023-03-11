NAME=libwc
COMPILER=gcc
FLAGS=-Wall

all:
	make static
	make dynamic

static:
	$(COMPILER) $(FLAGS) -c $(NAME).c
	ar cr $(NAME).a $(NAME).o

dynamic:
	$(COMPILER) $(FLAGS) -fPIC -c $(NAME).c
	$(COMPILER) $(FLAGS) -fPIC -shared -o $(NAME).so $(NAME).o

clean:
	rm -f *.o *.a *.so
	rm -f main