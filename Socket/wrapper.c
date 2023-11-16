/**
 * Realizzare una libreria di wrapper per le chiamate alle procedure per le socket:
   - socket, connect, bind, listen, accept
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include "wrapper.h"

int Socket(int family, int type, int protocol)
{
  int n;
  if ( (n = socket(family, type, protocol)) < 0) {
    perror("socket");
    exit(1);
  }

  return(n);
}


void Connect(int sockfd, struct sockaddr_in servaddr)
{
    if ( connect(sockfd, (struct sockaddr *) &servaddr, sizeof(servaddr)) < 0 ) {
      fprintf(stderr,"connect error\n");
      exit(1);
    }
}

void Bind(int listenfd, struct sockaddr_in servaddr)
{
  if ( bind(listenfd, (struct sockaddr *) &servaddr, sizeof(servaddr)) < 0 ){
    perror("bind");
    exit(1);
  }
}

void Listen(int listenfd, int num)
{
  if ( listen(listenfd, num) < 0 ) {
    perror("listen");
    exit(1);
  }
}


int Accept(int listenfd, struct sockaddr *servaddr, socklen_t *addrlen)
{
  int connfd;
  if ( connfd = accept(listenfd, servaddr, addrlen) < 0 ) {
      perror("accept");
      exit(1);
  }

  return connfd;
}