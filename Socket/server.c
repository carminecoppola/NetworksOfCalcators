/**
 * Utilizzando le funzioni wrapper
 * - riscrivere il client daytime
 * - riscrivere il server daytime
 * – test
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <time.h>
#include "wrapper.h"

int main(int argc, char **argv)
{
    int listenfd, connfd;
    struct sockaddr_in servaddr;
    char buff[4096];
    time_t ticks;

    //socket utilizzando wrapper
    listenfd = Socket(AF_INET, SOCK_STREAM, 0);
    /*if ( ( listenfd = socket(AF_INET, SOCK_STREAM, 0) ) < 0 ) {
        perror("socket");
        exit(1);
    }*/

    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(13);

    //funzione wrapper
    Bind(listenfd, (struct sockaddr_in) servaddr);
    /*if ( bind(listenfd, (struct sockaddr *) &servaddr, sizeof(servaddr)) < 0 )
    {
        perror("bind");
        exit(1);
    }*/


    //funzione wrapper
    Listen(listenfd, 1024);
    /*if ( listen(listenfd, 1024) < 0 ) {
        perror("listen");
        exit(1);
    }*/

    for ( ; ; ) {

        //funzione wrapper
        connfd = Accept(listenfd, (struct sockaddr *)NULL, NULL);
        /*if ( ( connfd = accept(listenfd, (struct sockaddr *) NULL, NULL) ) < 0 ) {
            perror("accept");
            exit(1);
        }*/
        
        ticks = time(NULL);
        snprintf(buff, sizeof(buff), "%.24s\r\n", ctime(&ticks));
        if ( write(connfd, buff, strlen(buff)) != strlen(buff)) {
            perror("write");
            exit(1);
        }
        close(connfd);
    }
}