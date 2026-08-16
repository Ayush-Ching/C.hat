# Terminal Chatting Application

A little chatting application written in `C` and allows any number of clients to connect to the server and chat.

Works only on UNIX based Operating Systems, so Windows users will have to use Virtual Machines or WSL.

-----

## Build Instructions

### Requirements
A `C` compiler like `GCC` or `Clang`

### Compiling
> To compile the server, if you have `GCC` compile it like this :
> ```bash
> gcc server.c socketutil.c -o server.out -pthread
> ```

> To compile the client, if you have `GCC` compile it like this :
> ```bash
> gcc client.c socketutil.c -o client.out -pthread
> ```

### Running
Simply run one instance of `server.out` and then run as many `client.out`s to connect to the server and chat.

You can edit the ip address of the server from the `client.c` file to target a server on any device.

-----
