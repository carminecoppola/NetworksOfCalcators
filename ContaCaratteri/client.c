/* Traccia:
    Si riscriva il server che conta i caratteri in modo che gestisca
    le connessioni mediante I/O mutliplex
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <errno.h>
#include <string.h>
#include <sys/select.h>

ssize_t FullRead(int fd, void *buf, size_t count);
ssize_t FullWrite(int fd, const void *buf, size_t count);


#define MAX_BUFFER_SIZE 1024

#define max(x, y) ({typeof (x) x_ = (x); typeof (y) y_ = (y); \
    x_ > y_ ? x_ : y_;})

int main(int argc, char **argv)
{
    int sockfd;
    struct sockaddr_in servaddr;
    fd_set set;
    int maxd;

    if (argc != 2)
    {
        fprintf(stderr, "usage: %s <IPaddress>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(1024);

    if (inet_pton(AF_INET, argv[1], &servaddr.sin_addr) <= 0)
    {
        perror("inet_pton");
        exit(EXIT_FAILURE);
    }

    if (connect(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0)
    {
        perror("connect");
        exit(EXIT_FAILURE);
    }

    char recvline[MAX_BUFFER_SIZE];
    char printBuffer[MAX_BUFFER_SIZE];

    while (1)
    {
        FD_ZERO(&set);
        FD_SET(STDIN_FILENO, &set);
        FD_SET(sockfd, &set);
        maxd = max(STDIN_FILENO, sockfd) + 1;

        if (select(maxd, &set, NULL, NULL, NULL) < 0)
            exit(1);

        if (FD_ISSET(STDIN_FILENO, &set))
        {
            /*leggi da standard input */
            if (fgets(printBuffer, MAX_BUFFER_SIZE, stdin) == NULL) 
                exit(EXIT_FAILURE);

            printBuffer[strlen(printBuffer)-1] = '\0';
            FullWrite(sockfd, printBuffer, sizeof(printBuffer));
            fflush(stdout);
            printf("Read from STDIN: %s\n", printBuffer);
        }

        if (FD_ISSET(sockfd, &set))
        {
            /*leggi da sockfd */
            FullRead(sockfd, recvline, sizeof(recvline));
            
            if (strlen(recvline) == 0) {
                printf("Server closed the connection. Exiting...\n");
                break;
            }
            printf("Read from socket: %s\n", recvline);
        
        }
    }

    
    close(sockfd);
    return 0;
}

ssize_t FullWrite(int fd, const void *buf, size_t count)
{
    size_t nleft = count;
    ssize_t nwritten;

    while (nleft > 0)
    {
        if ((nwritten = write(fd, buf, nleft)) < 0)
        {
            if (errno == EINTR)
            {
                continue;
            }
            else
            {
                perror("write");
                exit(EXIT_FAILURE);
            }
        }
        else if (nwritten == 0)
        {
            break; // Connessione chiusa dal peer
        }

        nleft -= nwritten;
        buf += nwritten;
    }

    return (count - nleft); // Restituisce il numero totale di byte scritti
}

ssize_t FullRead(int fd, void *buf, size_t count)
{
    size_t nleft = count;
    ssize_t nread;

    while (nleft > 0)
    {
        if ((nread = read(fd, buf, nleft)) < 0)
        {
            if (errno == EINTR)
            {
                continue;
            }
            else
            {
                perror("read");
                exit(EXIT_FAILURE);
            }
        }
        else if (nread == 0)
        {
            break; // Connessione chiusa dal peer
        }

        nleft -= nread;
        buf += nread;
    }

    return (count - nleft); // Restituisce il numero totale di byte letti
}