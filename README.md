# NetworksOfCalcators

This repository contains a series of exercises from the Computer Networks course implemented in the C language. Each exercise is organized in a separate folder, containing both the client and server code, or a peer-to-peer connection.

## Commands for running the server and client

Before running the server and client, make sure to compile the source files using the GCC compiler. Provide them with meaningful names:

  ```
  gcc nomeFileServer.c -o server
  gcc nomeFileClient.c -o client
  ```

These commands will compile the source files into executables with the names "server" and "client."

To start the server, run the following command to launch the server in the background:

  ```
  ./server &
  ```

To start the client and connect it to the server, run the following command, specifying the server's IP address (in the example, it is "127.0.0.1"):

  ```
  ./client 127.0.0.1
  ```

## Commands for killing processes

To list all running processes, you can use one of the following commands:

  ```
  ps -a
  ps -ax
  ```
To identify the server process, you can filter the output using the "grep" command like this:

  ```
  ps -ax | grep server
  ```
This command will provide you with the process name associated with the server.

To kill a process, use the "kill -9" command followed by the process name. For example:

  ```
  kill -9 nomeProcesso
  ```
## Running multiple commands with the "for" loop

To run the client repeatedly from the terminal, you can use a "for" loop. Here's an example:
  ```
  for i in {1..5}
  do
    ./client 127.0.0.1
  done
  ```

This will run the client five times, connecting it to the server with the IP address "127.0.0.1".
