all: gen_random tree test

gen_random: gen_random.c
	g++ -Wall -Werror -g -I. -o gen_random gen_random.c -pthread

tree: main.c arg_parser.c mutex_lock_bst.c rw_lock_bst.c
	g++ -I. -o tree main.c arg_parser.c mutex_lock_bst.c rw_lock_bst.c -pthread

test: unit_test.c arg_parser.c mutex_lock_bst.c
	g++ -I. -o test unit_test.c arg_parser.c mutex_lock_bst.c -pthread

clean:
	rm -f file tree test *.o
