CC = gcc
CFLAGS = -Wall -g

OBJS = main.o graph.o dijkstra.o linked_list.o

all: program

program: $(OBJS)
	$(CC) $(CFLAGS) -o program $(OBJS)

main.o: main.c graph.h dijkstra.h linked_list.h
	$(CC) $(CFLAGS) -c main.c

graph.o: graph.c graph.h linked_list.h
	$(CC) $(CFLAGS) -c graph.c

dijkstra.o: dijkstra.c dijkstra.h graph.h
	$(CC) $(CFLAGS) -c dijkstra.c

linked_list.o: linked_list.c linked_list.h
	$(CC) $(CFLAGS) -c linked_list.c

clean:
	rm -f program $(OBJS)
