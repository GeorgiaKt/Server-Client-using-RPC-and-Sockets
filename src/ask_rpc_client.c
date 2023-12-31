/*
 * This is sample code generated by rpcgen.
 * These are only templates and you can use them
 * as a guideline for developing your own functions.
 */

/*
	RPC Client code (works as a Socket Server at the same time)
	The file contains:
		- creating socket & connecting socket and RPC kind of communication between server-client
		- receiving data from client
		- calling the corresponding function, based on the data received
		- receiving results from the functions
		- returning results to client (client prints the results)
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include "ask_rpc.h"

void error(char *msg) // function for printing errors
{
	perror(msg);
	exit(1); // error code 1 for socket_server/ rpc_clien (0 for socket_client)
}

void ask1_prog_1(char *host, struct inputs inp, int socket) // function that calls the corresponding function based on the option chosen by client
{
	CLIENT *clnt;
	int *result_1;
	averages *result_2;
	mul_arr *result_3;

#ifndef DEBUG
	clnt = clnt_create(host, ASK1_PROG, ASK1_VERS, "udp");
	if (clnt == NULL)
	{
		clnt_pcreateerror(host);
		exit(1);
	}
#endif /* DEBUG */

	if (inp.choice == 1) // if option 1 is chosen
	{
		result_1 = calc_inner_pr_1(&inp, clnt); // call rpc's server function
		if (result_1 == (int *)NULL)
			clnt_perror(clnt, "call failed");
		else
			send(socket, result_1, sizeof(int), 0); // send result to client
	}
	else if (inp.choice == 2) // if option 2 is chosen
	{
		result_2 = find_avgs_1(&inp, clnt); // call rpc's server function
		if (result_2 == (averages *)NULL)
			clnt_perror(clnt, "call failed");
		else
			send(socket, result_2->avg.avg_val, 2 * sizeof(double), 0); // send result (array) to client
	}
	else if (inp.choice == 3) // if option 2 is chosen
	{
		result_3 = calc_mul_1(&inp, clnt); // call rpc's server function
		if (result_3 == (mul_arr *)NULL)
			clnt_perror(clnt, "call failed");
		else
			send(socket, result_3->array.array_val, result_3->array.array_len * sizeof(double), 0); // send result (array) to client
	}

#ifndef DEBUG
	clnt_destroy(clnt);
#endif /* DEBUG */
}

int main(int argc, char *argv[])
{
	char *host;

	int sockfd, newsockfd, portno;			// define socket for connection and new socket for communication with client, port number of the server
	int clilen, done;						// define local variable clilen for the length of client's ip, flag variable
	pid_t procId;							// process ID
	struct sockaddr_in serv_addr, cli_addr; // define Internet Address for Server and Client
	unsigned int clients = 0;				// number of clients (how many processes connected)

	int tr, tn, tX, tY, tch; // variables for send, recv

	int t, res1;

	if (argc < 3) // arguments provided: srerver_host, socket port
	{
		printf("usage: %s server_host socket_port\n", argv[0]);
		exit(1);
	}

	portno = atoi(argv[2]);					  // initialize port number based on argument
	sockfd = socket(AF_INET, SOCK_STREAM, 0); // create a socket for the clients to connect to the server
	if (sockfd < 0)
		error("ERROR opening socket");

	// initializing serv_addr's information which is a sockaddr_in struct
	bzero((char *)&serv_addr, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(portno);
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	host = argv[1]; // initialize server's host name

	if (bind(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) // bind the socket to a specific ip and port
		error("ERROR on binding");

	listen(sockfd, 5); // server starts to listen - waits up to 5 clients to connect to the socket

	for (;;) // server runs constantly
	{
		printf("Waiting for a connection...\n");
		clilen = sizeof(cli_addr);
		newsockfd = accept(sockfd, (struct sockaddr *)&cli_addr, &clilen); // create new socket for communication & accepts a client that tried to connect to the server
		if (newsockfd < 0)
			error("ERROR on accept");

		if (fork() == 0) // child process - code run by clients
		{
			close(sockfd); // close socket where clients connect
			printf("Client Connected.\n");

			inputs inp; // define struct that has: choice, r, n, X, Y

			done = 0;
			do
			{
				// receiving data from client and storing them in struct inp that is sent to rpc server afterwards
				//  choice
				tch = recv(newsockfd, &inp.choice, sizeof(inp.choice), 0);

				if (inp.choice == 4) // if the client chose option 4, the loop breaks and then the socket closes and rpc client exits
				{
					printf("Client Exited.\n");
					break;
				}

				// r
				tr = recv(newsockfd, &inp.r, sizeof(inp.r), 0);

				// n
				tn = recv(newsockfd, &inp.n, sizeof(inp.n), 0);

				// initializing
				inp.X.X_len = inp.Y.Y_len = inp.n; // length of X and Y is n
				// memory allocation for X and Y
				inp.X.X_val = (int *)malloc(inp.n * sizeof(int));
				inp.Y.Y_val = (int *)malloc(inp.n * sizeof(int));

				// X
				tX = recv(newsockfd, inp.X.X_val, inp.n * sizeof(int), 0);

				// Y
				tY = recv(newsockfd, inp.Y.Y_val, inp.n * sizeof(int), 0);

				// call function responsible for communicating with rpc server that calls the corresponding functions for the calculations
				ask1_prog_1(host, inp, newsockfd);

				if (tr < 1 || tn < 1 || tX < 1 || tY < 1 || tch < 1) // if receiving data fails then the loop breaks
					done = 1;
			} while (!done);
			close(newsockfd); // close the socket between server-client
			exit(0);		  // exit
		}

		// parent's code only - since it exits if its a child
		close(newsockfd); // close the socket where server and client communicate

		clients++;
		while (clients)
		{
			procId = waitpid((pid_t)-1, NULL, WNOHANG); // waitpid is used from parent in order to wait for all children to finish - no zombies
			if (procId < 0)
				error("ERROR on waitpid");
			else if (procId == 0)
			{
				break;
			}
			else
				printf("Client disconnected.\n"); // print one for each client run at the same time - at the end, after all clients disconnect
		}
	}
	exit(0);
}