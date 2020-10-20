all: counter

counter: main.cpp  sorts.cpp
	g++ -Wall -Werror -I. -o counter main.cpp sorts.cpp custom_locks.cpp -pthread

clean:
	rm -f counter *.o
