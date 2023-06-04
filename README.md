# Dijkstra's Shortest Path Algorithm in C

This project implements Dijkstra's shortest path algorithm using a simple representation of a graph in C. Started as a uni project but optimised and remade for general purpose.

## Project Structure

The project includes several source files:

- `graph.h` and `graph.c`: These files define a simple graph data structure and functions for creating and manipulating the graph.

- `linkedlist.h` and `linkedlist.c`: These files define a simple linked list data structure that is used in the graph implementation.

- `dijkstra.h` and `dijkstra.c`: These files implement Dijkstra's shortest path algorithm. The function `dijkstra()` takes a graph and a source node as input, and returns a table of shortest path information.

- `main.c`: This is the entry point of the program. It tests the Dijkstra's shortest path algorithm implementation with a few graph examples.

- `Makefile`: This file is used to automate the build process of the project. 

## How to Run

To run the project, use the `make` command to compile the program and then run the executable:

```bash
make
./program
```
## Testing

To test, design the graph you want as shown in the example in main.c, then make and run the file


# Open to contribution
