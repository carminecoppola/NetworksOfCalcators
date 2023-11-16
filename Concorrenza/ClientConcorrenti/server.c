#include<sys/types.h> /* predefined types */
#include<unistd.h> /* include unix standard library */
#include<arpa/inet.h> /* IP addresses conversion utililites */
#include<sys/socket.h> /* socket library */
#include<stdio.h> /* include standard I/O library */
#include <stdlib.h>
#include <string.h>
#include<time.h>
#include <errno.h>


ssize_t FullWrite(int fd, const void *buf, size_t count);


int main (int argc , char *argv[])
{
    int list_fd,conn_fd;
    int i;
    struct sockaddr_in serv_add,client;
    char buffer [1024];
    socklen_t len;
    time_t timeval;
    pid_t pid;
    int logging =1;

    
     
    if ( ( list_fd = socket(AF_INET, SOCK_STREAM, 0) ) < 0 ) {
        perror("socket");
        exit(1);
    }
    
    serv_add.sin_family      = AF_INET;
    serv_add.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_add.sin_port        = htons(1024);

    if ( bind(list_fd, (struct sockaddr *) &serv_add, sizeof(serv_add)) < 0 ) {
        perror("bind");
        exit(1);
    }

    if ( listen(list_fd, 1024) < 0 ) {
        perror("listen");
        exit(1);
    }

    while(1)
    {
        len = sizeof( client );
        if ( ( conn_fd = accept(list_fd, (struct sockaddr *) NULL, NULL) ) < 0 ) {
            perror("accept");
            exit(1);
        }

        
        /* fork to handle connection */
        if( ( pid = fork() ) < 0 )
        {
            perror (" fork error ");
            exit ( -1);
        }
        if( pid == 0 )
        { 
            /* child */
            close ( list_fd );
            timeval = time ( NULL );
            snprintf(buffer,sizeof(buffer)," %.24s\r\n",ctime(& timeval));
            
            FullWrite(conn_fd, buffer, sizeof(buffer));
            
            if(logging)
            {
                inet_ntop(AF_INET,&client.sin_addr,buffer,sizeof(buffer));
                printf("Request from host %s, port %d\n",buffer,ntohs(client.sin_port));
            }

            close(conn_fd);
            /*COMPLETE HERE*/
            exit (0);
        }
        else 
        { 
            /* parent */
            close ( conn_fd );
        }
    }

    /* normal exit , never reached */
    exit (0);
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