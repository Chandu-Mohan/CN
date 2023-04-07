libmysocket.a: mysocket.o
	ar -rcs libmysocket.a mysocket.o

mysocket.o: mysocket.c mysocket.h
	gcc -c mysocket.c -o mysocket.o -lpthread

clean:
	rm -f mysocket.o libmysocket.a
