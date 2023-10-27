#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <time.h>
#include <errno.h>

ssize_t FullWrite(int fd, const void *buf, size_t count);

int main(int argc, char **argv)
{
    int          listenfd, connfd;
    struct sockaddr_in  servaddr;
    char        buff[4096];
    time_t        ticks;  
    if ( ( listenfd = socket(AF_INET, SOCK_STREAM, 0) ) < 0 ) {
        perror("socket");
        exit(1);
        }
    servaddr.sin_family      = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port        = htons(1024);
    if ( bind(listenfd, (struct sockaddr *) &servaddr, sizeof(servaddr)) < 0 ) {
        perror("bind");
        exit(1);
    }
    if ( listen(listenfd, 1024) < 0 ) {
        perror("listen");
        exit(1);
    }
    for ( ; ; ) {
        if ( ( connfd = accept(listenfd, (struct sockaddr *) NULL, NULL) ) < 0 ) {
            perror("accept");
            exit(1);
        }
        ticks = time(NULL);
        snprintf(buff, sizeof(buff), "%.24s\r\n", ctime(&ticks));

        FullWrite(connfd, buff, sizeof(buff));

        close(connfd);
    }
}

ssize_t FullWrite(int fd, const void *buf, size_t count)  
{ 
    size_t nleft; 
    ssize_t nwritten; 
    nleft = count; 
    while (nleft > 0) {             /* repeat until no left */ 
        if((nwritten = write(fd, buf, nleft))<0){
            if(errno == EINTR){
                continue;
            }
            else{
                exit(EXIT_FAILURE);
            }
        }
        else{
            nleft -= nwritten;
            buf += nwritten;
        }    
    } 
    return (nleft); 
}