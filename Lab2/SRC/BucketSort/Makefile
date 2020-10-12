all: mysort

mysort: main.cpp  sorts.cpp
	g++ -Wall -Werror -I. -o mysort main.cpp sorts.cpp custom_locks.cpp -pthread

clean:
	rm -f mysort *.o
