# Server-Client using RPC and Sockets

## Description
  In this project there is a Server-Client communication, combining RPC and Sockets. Starting with the socket communication, there is a client that is responsible for communicating with the user, meaning printing menu, reading data and printing back the results calculated and there is also a server. The socket server is, also, a RPC client at the same time. So, when the socket server receives the data sent from the client (socket client), then calls a function responsible for the corresponding calculations. The RPC server is the one that has the functions responsible for calculating the results.

Main data types that are used: vectors X and Y, variable n for the length of both vectors, variable r for a fixed value given.

The Calculations that can be made:
- Inner Product: (X*Y)
- Average Values: Ex, Ey (for each vector)
- Product: r*(X+Y)

## Requirements
- RPC installed
- Linux environment or WSL

## How to compile
```
    gcc client.c -o client
    make
```
 
## How to run
```
    ./ask_rpc_server
    ./ask_rpc_client localhost <port>
    ./client localhost <port>
```
*Each command should be executed in a different terminal.*
  
Before running make sure you have run `sudo rpcbind`!
