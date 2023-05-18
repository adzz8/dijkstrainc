# COMP1005 2022 Coursework 06

## Introduction

This part of the coursework focuses on implementing a graph library using linked lists. There is one task to complete, details of which are given below. Links to external pages are provided in the details below which you will need to follow and read to complete the task.

## Overview

In this task you will implement a number of library functions. An executable program is provided which will include and use the library functions which you implement. You will not need to add any files to your forked repository for this task, as all the required files are present. You will add function implementations to certain source files in your repository. A `Makefile` is also provided to build and test your library functions. All code will be compiled with the gcc `-ansi -pedantic-errors` switches.

## Assessment

This coursework is worth **20%** of your final COMP1005 grade. The points awarded for Task 1 are as follows:

|            | Compilation | Implementation | Execution  | Bonus | **Total** |
| ---------- | :---------: | :------------: | :-------:  | :---: | :-------: |
| **Task 1** | 2           | 3              | 10         | 5     | **20**    |


For Task 1, you are awarded *two* points if your program compiles correctly: *one* point if it compiles without errors, and a further *one* point if it compiles without errors and warnings. If your program follows the implementation instructions given in the task details below, you are awarded *three* points.

If your program is implemented so that no memory leaks occur when it is executed, you are awarded *five* points. If your program produces the correct output, you are awarded a further *five* points. Additionally, if you implement COMP1006 Task 2 (Dijkstra's algorithm) using your graph library implemented in Task 1, you will be awarded an additional *five* bonus points. Bonus points will not be shown in the pipeline.

The contribution towards your final COMP1005 grade is calculated as follows:

```maths
	floor(20 * task1_points_awarded / 20)
```

Your provisional score for each task can be viewed on GitLab after every push to `projects.cs.nott.ac.uk`. Your final score will be based on the code in the last commit pushed to `projects.cs.nott.ac.uk` *before* the coursework deadline. Commits pushed after the coursework deadline will be disregarded. After the coursework deadline, your code will undergo further review and, based on this review, your provisional score may go up or down. This further review will include checks for code plagiarism and for trivial implementations e.g. implementations just containing an empty main function or clearly not written following the task guidelines. Final scores will be published on the COMP1005 moodle pages around a week after the coursework deadline.

Your repository contains a file called `.gitlab-ci.yml`. This file is used during the assessment process and *must not* be removed or edited in any way. Any tampering with this file will result in a score of zero for this coursework.

This coursework is individual work i.e. *must be your own work*.

## Task 1

### Overview

Your task is to implement a [graph](https://en.wikipedia.org/wiki/Graph_(abstract_data_type)) library.

A test program `task1_test.c` and graph library header file `graph.h` are provided for you. You must *not* remove or edit these two files in any way.

### Details

Graphs are fundamental data structures in Computer Science. They are commonly implemented using a linked list using an [adjacency list](https://en.wikipedia.org/wiki/Adjacency_list) representation. You will use the implementation of a linked list provided to implement a directed graph library using adjacency lists.

You do *not* need to implement a linked list library, as a full implementation is provided in the `linked_list.h` and `linked_list.c` files. You must not edit these two files in any way. You should use this implementation when implementing your graph library.

The header file which contain the interface for an implementation of a graph library in C is given in the `graph.h` file. This file contains the structures you will use to represent a graph, vertices, and edges. You will not need to implement any further structures.

A skeleton implementation file `graph.c` which implements each of the functions declared the interface file is also provided. Your task is to complete the implementation of the functions in `graph.c`. You should only edit the `graph.c` file, and must *not* edit the `graph.h` file. You are free to implement the internals of each function in any way you want, but you must *not* change the function definitions in the provided `graph.c` file i.e. you must *not* change the function names, return types or parameters. All warnings and error messages in your implementation should be printed to `stderr`. Further implementation guidelines are given in the comments in `graph.c`.

A test program, `task1_test.c`, is provided which will include your implementation and use your implementation to manipulate a graph. You must *not* edit the file `task1_test.c` or `utility.h`. To compile your implementation and the test program, type:

```bash
$ make clean task1_test
```

To compile your implementation and the test program and then run the test program, type:

```bash
$ make clean task1_test_run
```

A typical output (your warning messages may vary) from a correctly working implementation is:

```bash
$ make clean task1_test_run
rm -f task1_test *.o
gcc -c linked_list.c -Wall -ansi -pedantic-errors -g
gcc -c graph.c -Wall -ansi -pedantic-errors -g
gcc -o task1_test task1_test.c linked_list.o graph.o -Wall -ansi -pedantic-errors -g
./task1_test 1:3:1 1:5:2 2:3:10 2:4:2 3:4:2
Performing Test 1...
warning: unable to remove vertex
warning: unable to find vertex
warning: unable to remove edge
warning: unable to find vertex
warning: unable to remove edge
warning: unable to add vertex
warning: unable to add edge
warning: unable to add undirected edge
warning: unable to find vertex
warning: unable to print graph
warning: unable to free edge
warning: unable to free vertex
warning: unable to free graph
Completed Test 1.
Performing Test 2...
initialising graph...
adding edges...
removing graph...
Completed Test 2.
Performing Test 3...
initialising graph...
adding edges...
removing edges...
removing graph...
Completed Test 3.
Performing Test 4...
initialising graph...
adding edges...
removing vertices...
warning: unable to remove vertex
warning: unable to remove vertex
removing graph...
Completed Test 4.
Performing Test 5...
initialising graph...
adding edges...
printing graph...
1: 3[1.00] 5[2.00]
3: 1[1.00] 2[10.00] 4[2.00]
5: 1[2.00]
2: 3[10.00] 4[2.00]
4: 2[2.00] 3[2.00]
removing graph...
Completed Test 5.
Performing Test 6...
initialising graph...
adding edges...
removing edges...
adding edges...
printing graph...
1: 3[1.00] 5[2.00]
3: 1[1.00] 2[10.00] 4[2.00]
5: 1[2.00]
2: 3[10.00] 4[2.00]
4: 2[2.00] 3[2.00]
removing graph...
Completed Test 6.
Performing Test 7...
initialising graph...
adding edges...
printing graph...
1: 3[1.00] 5[2.00]
3: 1[1.00] 2[10.00] 4[2.00]
5: 1[2.00]
2: 3[10.00] 4[2.00]
4: 2[2.00] 3[2.00]
removing graph...
Completed Test 7.
```

Your implementation should handle dynamically allocated memory correctly i.e. free all dynamically allocated memory. To check your implementation using `valgrind` you can type:

```bash
$ make task1_test_memcheck
```

If your program has correctly handled dynamic memory allocation, the last line of output should read:

```
ERROR SUMMARY: 0 errors from 0 contexts (suppressed: 0 from 0)
```