#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <sys/socket.h>
#include <float.h>
/* You will also need to add #include for your graph library header files */
#include "graph.h"
#include "dijkstra.h"
#define kBufSize 12
#define WAITING 0
#define QUIT 4

void ServerConnection(int sd, Graph* net_graph);
void ReadLineFromNetwork(int sd, char *buf, int size);

int main(int argc, const char * argv[])
{
	Graph* net_graph = init_graph();
	int serverSocket, clientConnection;
	struct sockaddr_in server;
	struct sockaddr_in client;
	unsigned int alen; 

	printf("Programming Portfolio 2022 Implementation\n");
	printf("=========================================\n\n");

	/* Set up socket */
	serverSocket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
	
	if(serverSocket == -1)
	{
		fprintf(stderr, "Cannot create socket\n");	
		exit(1);
	}
	
	/* Setup socket address */
	memset(&server, 0, sizeof(server));
	server.sin_family = AF_INET;
	server.sin_port = htons(atoi(argv[1]));
	server.sin_addr.s_addr = INADDR_ANY;
	
	if(bind(serverSocket, (struct sockaddr *)&server, sizeof(server)) < 0)
	{
		printf("bind() failed\n");
		
		exit(2);
	}
	
	
	if(listen(serverSocket, 15) < 0)
	{
		printf("listen() failed\n");
		
		exit(3);
	}
	
	
	while(1)
	{
		alen = sizeof(client);
		clientConnection = accept(serverSocket, (struct sockaddr *)&client, &alen);
		printf("Connection From %x on port %d\n", ntohl(client.sin_addr.s_addr), ntohs(client.sin_port));

		
		/* Communicate with the client */
		ServerConnection(clientConnection, net_graph);
	
	
		/* Close connection with the client*/
		close(clientConnection);
		free_graph(net_graph);
	
	}
	return 0;
}

void ServerConnection(int sd, Graph* net_graph)
{
	char outBuffer[256];
	char inBuffer[256];
	int state = WAITING;
	char* token;
	char* sourceID;
	char* destID;
	char* weight;
    double weightcurrent;
	int count = 0, i = 0, edgeCount = 0;
    int* Vertices;
    Path* paths;
    int PnEntries = 256;

	
	sprintf(outBuffer, "+OK Programming Portfolio 2022 Implementation\r\n");
	write(sd, outBuffer, strlen(outBuffer));
	
	while(state != QUIT)
	{
		ReadLineFromNetwork(sd, inBuffer, 256);
		printf("[%s] received\n", inBuffer);

		if (strcmp(inBuffer, "QUIT") == 0) {
			sprintf(outBuffer,"+OK\r\n");
			state = QUIT;
				
		} else if (strstr(inBuffer, "NET-ADD")) {
			token = strtok(inBuffer, " ");
			token = strtok(NULL, " ");
            if(!token || atoi(token) == 0){
                sprintf(outBuffer, "-ERR Invalid Network ID\r\n");
            } else if (!find_vertex(net_graph, atoi(token))) {
                if(!add_vertex(net_graph, atoi(token))) {
                    sprintf(outBuffer, "-ERR  Failed to add network\r\n");
                }
                else {
                    sprintf(outBuffer, "+OK Added%2d\r\n",atoi(token));
                }
            } else if (find_vertex(net_graph, atoi(token))) {
				sprintf(outBuffer, "-ERR Network ID already exists\r\n");
			}
			
		} else if (strstr(inBuffer, "NET-DELETE")) {
            token = strtok(inBuffer, " ");
            token = strtok(NULL, " ");
            if(!token || atoi(token) == 0) {
                sprintf(outBuffer, "-ERR Invalid Network ID\r\n");
            } else {
                if(find_vertex(net_graph, atoi(token))) {
                    remove_vertex(net_graph, atoi(token));
                    sprintf(outBuffer, "+OK Network removed \r\n");

                } else {
                    sprintf(outBuffer, "-ERR Network invalid\r\n");
                }
            }
		} else if (strcmp(inBuffer, "NET-LIST") == 0) {
            Vertices = get_vertices(net_graph, &count);
            sprintf(outBuffer, "+OK %d\r\n", Vertices[1]);
			
		} else if (strstr(inBuffer, "ROUTE-ADD")) {
            token = strtok(inBuffer, " ");
            sourceID = strtok(NULL, " ");
            destID = strtok(NULL, " ");
            weight = strtok(NULL, " ");
            
            if(!sourceID || !destID || !weight || atoi(sourceID) == 0 || atoi(destID) == 0 || atoi(weight) == 0) {
                sprintf(outBuffer, "-ERR Source ID / Destination ID / Weight not valid \r\n");
            } else {
                if(find_vertex(net_graph, atoi(sourceID)) && find_vertex(net_graph, atoi(destID))) {
                    sprintf(outBuffer, "+OK Route Added \r\n");
                    if(NULL != get_edge(net_graph, atoi(sourceID), atoi(destID))) {
                        remove_edge(net_graph, atoi(sourceID), atoi(destID));
                        remove_edge(net_graph, atoi(destID), atoi(sourceID));
                    }
                    add_edge_undirected(net_graph, atoi(sourceID), atoi(destID), atoi(weight));
                } else {
                    sprintf(outBuffer, "-ERR Network Invalid \r\n");
                }
            }
			
        } else if (strstr(inBuffer, "ROUTE-DELETE")) {
            token = strtok(inBuffer, " ");
            sourceID = strtok(NULL, " ");
            destID = strtok(NULL, " ");
            
            if(!sourceID || !destID || atoi(sourceID) == 0 || atoi(destID) == 0) {
                sprintf(outBuffer, "-ERR invalid source / Destination ID / weight\r\n");
            } else {
                if(find_vertex(net_graph, atoi(sourceID)) && find_vertex(net_graph, atoi(destID))) {
                    if(get_edge(net_graph, atoi(sourceID), atoi(destID))) {
                        remove_edge(net_graph, atoi(sourceID), atoi(destID));
                        remove_edge(net_graph, atoi(destID), atoi(sourceID));
                        sprintf(outBuffer, "+OK Route Deleted\r\n");
                    } else {
                        sprintf(outBuffer, "-ERR Route invalid\r\n");
                    }
                }
            }
        } else if (strstr(inBuffer, "ROUTE-SHOW")) {
            token = strtok(inBuffer, " ");
            sourceID = strtok(NULL, " ");
            Vertices = get_vertices(net_graph, &count);
            if(!find_vertex(net_graph, atoi(sourceID))) {
                sprintf(outBuffer, "-ERR Network Invalid \r\n");
            } else {
                for(i = 0; i < count; i++) {
                    if(get_edge(net_graph, atoi(sourceID), Vertices[i])) {
                        edgeCount ++;
                    }
                }
                sprintf(outBuffer, "+OK %d\r\n", edgeCount);
                edgeCount = 0;
            }
        } else if (strstr(inBuffer, "ROUTE-HOP")) {
            token = strtok(inBuffer, " ");
            sourceID = strtok(NULL, " ");
            destID = strtok(NULL, " ");
            
            if(!sourceID || !destID || atoi(sourceID) == 0 || atoi(destID) == 0 || sourceID == destID) {
                sprintf(outBuffer, "-ERR Source ID / Destination ID not valid \r\n");
            } else {
                paths = dijkstra(net_graph, atoi(sourceID),&PnEntries);
                sprintf(outBuffer,"+OK %d\r\n",paths[atoi(destID)].next_hop);
            }
        } else if (strstr(inBuffer, "ROUTE-TABLE")) {
            int offset = 0;
            token = strtok(inBuffer, " ");
            sourceID = strtok(NULL, " ");
            if(!sourceID || atoi(sourceID) == 0) {
                offset += sprintf(&outBuffer[offset], "-ERR Invalid Network ID \r\n");
            } else if (!find_vertex(net_graph, atoi(sourceID))) {
                offset += sprintf(&outBuffer[offset], "-ERR Network ID not found \r\n");
            } else {
                Path* paths = dijkstra(net_graph, atoi(sourceID), &PnEntries);
                offset += sprintf(&outBuffer[offset], "+OK %d\r\n", PnEntries);
                for(i = 0; i < PnEntries; i++) {
                    if(i == atoi(sourceID)) {
						continue;
                    } else {
                        weightcurrent = paths[i].weight;
                        if(weightcurrent == DBL_MAX) {
							continue;
                        } else {
                            offset += sprintf(&outBuffer[offset], "%d -> %d, next-hop %d, weight %d\r\n", atoi(sourceID), i, paths[i].next_hop, (int)weightcurrent);
                        }
                    }
                }
            }
        
        } else {
            sprintf(outBuffer, "-ERR Not Implemented\r\n");
        }
        write(sd, outBuffer, strlen(outBuffer));
    }
}


void ReadLineFromNetwork(int sd, char *buf, int size)
{
	char l[kBufSize];
	int n,i = 0, j = 0;
	int line = 0;
	
	do
	{
		n = read(sd, l, kBufSize);
		for(i = 0; i < n; i++)
		{
			buf[j] = l[i];
			if(buf[j] == 10 && buf[j-1] == 13)
			{
				buf[j-1] = '\0';
				line = 1;
				break;
			}
			j++;
		}
	} while(line == 0 && n > 0);
}