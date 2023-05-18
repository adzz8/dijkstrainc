CC=clang
CFLAGS=-Wall -ansi -pedantic-errors -g -o graph -fno-omit-frame-pointer -fsanitize=undefined
CLIBS=

TASK1_TEST=1:3:1 1:5:2 2:3:10 2:4:2 3:4:2

all: clean task1_test task1_test_run

# task 1

task1_test: task1_test.c linked_list.c graph.c
	$(CC) -o task1_test $^ $(CFLAGS),memory $(CLIBS)
	$(CC) -o task1_test2 $^ $(CFLAGS),address $(CLIBS)

task1_test_run: task1_test
	./task1_test $(TASK1_TEST)
	./task1_test2 $(TASK1_TEST)

task1_test_memcheck: task1_test
	valgrind --tool=memcheck --leak-check=yes --show-reachable=yes ./task1_test $(TASK1_TEST)

clean:
	rm -f task1_test *.o task1_test2