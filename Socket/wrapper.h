#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>


int Socket(int family, int type, int protocol);
void Connect(int sockfd, struct sockaddr_in servaddr);
void Bind(int listenfd, struct sockaddr_in servaddr);
void Listen(int listenfd, int num);
int Accept(int listenfd, struct sockaddr *servaddr, socklen_t *addrlen);