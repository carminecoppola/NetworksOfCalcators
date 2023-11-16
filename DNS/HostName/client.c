/*
    Modificare il client daytime in modo che accetti anche nomi simbolici come parametro
    
    Modificare il server daytime in modo che visualizzi sullo standard output l'hostname dei client che lo contattano
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <errno.h>
#include <signal.h>
#include <netdb.h>


ssize_t FullRead(int fd, void *buf, size_t count);


int main(int argc, char **argv){

  int                sockfd, n;
  char               recvline[1025] ;
  struct sockaddr_in servaddr;


  if (argc != 2) {
    fprintf(stderr,"usage: %s <IPaddress>\n",argv[0]);
    exit (1);
  }

  if ( (sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
    fprintf(stderr,"socket error\n");
    exit (1);
  }

  servaddr.sin_family = AF_INET;
  servaddr.sin_port   = htons(1024);


  struct hostent *addr = gethostbyaddr((const char *)argv[1], sizeof(servaddr), servaddr.sin_family);

  if (inet_pton(AF_INET, (char*)addr, &servaddr.sin_addr) < 0) {
    fprintf(stderr,"inet_pton error for %s\n", argv[1]);
    exit (1);
  }

  if (connect(sockfd, (struct sockaddr *) &servaddr, sizeof(servaddr)) < 0) {
    fprintf(stderr,"connect error\n");
    exit(1);
  }
  
  FullRead(sockfd, recvline, sizeof(recvline));

  if (fputs(recvline, stdout) == EOF) {
    fprintf(stderr,"fputs error\n");
    exit(1);
  }

  exit(0);
}

ssize_t FullRead(int fd, void *buf, size_t count)  
{ 
  size_t nleft; 
  ssize_t nread; 
  nleft = count; 

  while (nleft > 0) {             /* repeat until no left */  
    if((nread = read(fd, buf, nleft)) < 0){
          if(errno == EINTR){
            continue;
          }
          else{
            exit(EXIT_FAILURE);
          }
    }

    if(nread == 0) break;
    else{
      nleft -= nread;
      buf += nread;
    }   
  }

  buf = 0; 
  
  return (nleft); 
}