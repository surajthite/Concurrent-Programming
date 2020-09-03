all: mysort

mysort: main.c  sorts.c
	cc -Wall -Werror -I. -o mysort main.c sorts.c

clean:
	rm -f mysort *.o
