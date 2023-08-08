/*
STEPS:

generating basic rpc code:
    rpcgen -a -C rpc.x
changing flags in Makefile.ask_rpc:
        CC=gcc
        CFLAGS += -g -I/usr/include/tirpc
        LDLIBS += -lnsl -ltirpc
        RPCGENFLAGS= -C
compile:
    make -f Makefile.ask_rpc
renaming Makefile:
    mv Makefile.ask_rpc Makefile


compiling:
    gcc client.c -o client
    make
enabling rpc:
    sudo rpcbind
running:
    ./ask_rpc_server
    ./ask_rpc_client localhost 3000
    ./client localhost 3000

*/

/*
    Socket Client Code
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>

void error(char *msg) // function for printing errors
{
    perror(msg);
    exit(0); // error code 0 for client (1 for server)
}

int main(int argc, char *argv[])
{
    int sockfd, portno;           // define socket for connection with server, port number of the server, flag variable
    struct sockaddr_in serv_addr; // define Internet Address for Server
    struct hostent *server;       // srtuct where host's information is stored

    // variables that are initialized by the user
    double r;
    int n;
    int *X;
    int *Y;
    int choice;

    // variables for results
    int t, resInner;
    double *resAvg, *resMul;

    if (argc < 3) // arguments provided: hostname, socket port
    {
        fprintf(stderr, "usage %s hostname port\n", argv[0]);
        exit(0);
    }

    portno = atoi(argv[2]);                   // initialize port number based on argument
    sockfd = socket(AF_INET, SOCK_STREAM, 0); // create a socket for the server-client communication
    if (sockfd < 0)
        error("ERROR opening socket");

    server = gethostbyname(argv[1]); // initialize host's information
    if (server == NULL)
    {
        fprintf(stderr, "ERROR, no such host\n");
        exit(0);
    }

    // initializing serv_addr's information which is a sockaddr_in struct
    bzero((char *)&serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(portno);
    bcopy((char *)server->h_addr, (char *)&serv_addr.sin_addr.s_addr, server->h_length);

    printf("Trying to connect...\n");

    // connecting to the server's socket - waits to be accepted
    if (connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
        error("ERROR connecting");

    printf("Connected.\n");

    do
    {
        // printing Menu
        printf("\n\nMENU:\n(Choose one of the options below)\n");
        printf("1. Calculate Inner Product: (X*Y)\n");
        printf("2. Find the Average Values: Ex, Ey\n");
        printf("3. Calculate the Product: r*(X+Y)\n");
        printf("4. Exit\n\n");

        // read choice of menu from user
        scanf("%d", &choice);

        if (choice != 1 && choice != 2 && choice != 3 && choice != 4) // if chosen option is none of the menu's then print menu again and wait for new option to be given
        {
            printf("Invalid Option, try again !\n");
            continue; // break this loop and continue to the next one
        }

        // send valid option to socket_server/ rpc_client
        send(sockfd, &choice, sizeof(choice), 0);

        if (choice == 4) // if option 4 is chosen, socket closes and client exits
        {
            printf("Exiting...\n");
            close(sockfd);
            exit(0);
        }

        // reading data from user
        // r
        printf("Enter float number (r): ");
        scanf("%lf", &r);               // read r
        send(sockfd, &r, sizeof(r), 0); // send r to socket_server/ rpc_client

        // n
        printf("Enter length of array (n): ");
        scanf("%d", &n);                // read n
        send(sockfd, &n, sizeof(n), 0); // send n to socket_server/ rpc_client

        // memory allocation for X and Y
        X = (int *)malloc(n * sizeof(int));
        Y = (int *)malloc(n * sizeof(int));
        if (X == NULL || Y == NULL)
            error("ERROR Memory Allocation Failed\n");

        // X
        printf("Enter %d integers for X array:\n", n);
        for (int i = 0; i < n; i++)
            scanf("%d", &X[i]);              // read X values
        send(sockfd, X, n * sizeof(int), 0); // send X to socket_server/ rpc_client

        // Y
        printf("Enter %d integers for Y array:\n", n);
        for (int i = 0; i < n; i++)
            scanf("%d", &Y[i]);              // read Y values
        send(sockfd, Y, n * sizeof(int), 0); // send Y to socket_server/ rpc_client

        printf("\n");

        // receiving & printing results based on the option chosen
        printf("Results:\n");
        if (choice == 1)
        {
            t = recv(sockfd, &resInner, sizeof(resInner), 0);
            printf("Inner Product (X*Y)= %d\n", resInner);
        }
        else if (choice == 2)
        {
            resAvg = (double *)malloc(2 * sizeof(double));
            t = recv(sockfd, resAvg, 2 * sizeof(double), 0);
            printf("Average Values Ex= %.3lf, Ey= %.3lf\n", resAvg[0], resAvg[1]);
        }
        else if (choice == 3)
        {
            resMul = (double *)malloc(n * sizeof(double));
            t = recv(sockfd, resMul, n * sizeof(double), 0);
            printf("Product r*(X+Y):\n");
            for (int i = 0; i < n; i++)
                printf("arr[%d]= %.3lf\n", i, resMul[i]);
        }

        if (t < 1) // if receiving data fails then the loop breaks
            break;
    } while (choice != 4); // loop breaks when client chooses to exit (option 4)

    // freeing memory that was dynamically allocated
    free(X);
    free(Y);

    close(sockfd); // close server-client's socket

    return 0;
}