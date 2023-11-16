/* Traccia:
    Dato il server concorrente che accetta in input stringhe dai client e
    restituisce il numero di caratteri per stringa si modifichi il client
    relativo in modo che gestista lo standard input e il socket di 
    connessione con il server mediante I/O mutliplex
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

int main(int argc, char **argv)
{
    int sockfd;
    struct sockaddr_in servaddr;
    fd_set set;
    int connection_closed = 0;

    if (argc != 3)
    {
        fprintf(stderr, "usage: %s <IPaddress> <string>\n", argv[0]);
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
    strncpy(string, argv[2], sizeof(string) - 1);
    string[sizeof(string) - 1] = '\0';

    int maxd;

    while (!connection_closed)
    {
        FD_ZERO(&set);
        FD_SET(STDIN_FILENO, &set);
        FD_SET(sockfd, &set);

        if (sockfd > STDIN_FILENO)
            maxd = sockfd + 1;
        else
            maxd = STDIN_FILENO + 1;

        if (select(maxd, &set, NULL, NULL, NULL) < 0)
            exit(EXIT_FAILURE);

        if (FD_ISSET(sockfd, &set))
        {
            // Poi leggi il numero di caratteri
            char recvline[100];
            if (FullRead(sockfd, recvline, sizeof(recvline)) == 0)
            {
                fprintf(stderr, "Server disconnected\n");
                connection_closed = 1;
            }

            if (fputs(recvline, stdout) == EOF)
            {
                fprintf(stderr, "fputs error\n");
                exit(EXIT_FAILURE);
            }
            printf("Sono entrato nel 1\n");
        }

        if (FD_ISSET(STDIN_FILENO, &set))
        {
            /*leggi da standard input */

            char input[100];

            if (fgets(input, sizeof(input), stdin) == NULL)
            {
                fprintf(stderr, "Error reading from stdin\n");
                exit(EXIT_FAILURE);
            }

            printf("Sono entrato nel 2\n");

            // Invia la stringa al server
            FullWrite(sockfd, input, strlen(input));
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
