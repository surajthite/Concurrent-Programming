all: counter

counter: counter.cpp  sorts.cpp
	g++ -Wall -Werror -I. -o counter counter.cpp sorts.cpp custom_locks.cpp -pthread
	mv counter ..

clean:
	rm -f counter *.o
