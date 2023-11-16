#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <errno.h>
#include <string.h>

ssize_t FullRead(int fd, void *buf, size_t count);
ssize_t FullWrite(int fd, const void *buf, size_t count);

int main(int argc, char **argv)
{
    int sockfd;
    struct sockaddr_in servaddr;

    if (argc != 3)
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

    if (inet_pton(AF_INET, argv[1], &servaddr.sin_addr) < 0)
    {
        perror("inet_pton");
        exit(EXIT_FAILURE);
    }

    if (connect(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0)
    {
        perror("connect");
        exit(EXIT_FAILURE);
    }

    char string[100];
    snprintf(string, sizeof(string), "%s", argv[2]);

    // Invia la stringa al server
    FullWrite(sockfd, string, sizeof(string));

    // Poi leggi il numero di caratteri
    char recvline[100];
    FullRead(sockfd, recvline, sizeof(recvline));

    if (fputs(recvline, stdout) == EOF)
    {
        fprintf(stderr, "fputs error\n");
        exit(1);
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