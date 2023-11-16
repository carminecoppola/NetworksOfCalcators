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

int main(int argc, char **argv) {
    int list_fd, conn_fd;
    struct sockaddr_in serv_addr, client_addr;
    socklen_t len = sizeof(client_addr);
    char buffer[1024];
    
    if ((list_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(1024);

    if (bind(list_fd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        perror("bind");
        exit(EXIT_FAILURE);
    }

    if (listen(list_fd, 5) < 0) {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    while (1) {
        if ((conn_fd = accept(list_fd, (struct sockaddr *)&client_addr, &len)) < 0) {
            perror("accept");
            exit(EXIT_FAILURE);
        }

        //Leggi la stringa del client
        char clientMessage[100];
        FullRead(conn_fd, clientMessage, sizeof(clientMessage));

        // Calcola il numero di caratteri nella stringa ricevuta
        int charCount = strlen(clientMessage);

        // Invia il numero di caratteri al client
        char response[100];
        snprintf(response, sizeof(response), "Numero di caratteri nella stringa: %d\n", charCount);
        FullWrite(conn_fd, response, strlen(response));

        close(conn_fd);
    }

    close(list_fd);
    return 0;
}

ssize_t FullWrite(int fd, const void *buf, size_t count) {
    size_t nleft = count;
    ssize_t nwritten;

    while (nleft > 0) {
        if ((nwritten = write(fd, buf, nleft)) < 0) {
            if (errno == EINTR) {
                continue;
            } else {
                perror("write");
                exit(EXIT_FAILURE);
            }
        } else if (nwritten == 0) {
            break; // Connessione chiusa dal peer
        }

        nleft -= nwritten;
        buf += nwritten;
    }

    return (count - nleft); // Restituisce il numero totale di byte scritti
}

ssize_t FullRead(int fd, void *buf, size_t count) {
    size_t nleft = count;
    ssize_t nread;

    while (nleft > 0) {
        if ((nread = read(fd, buf, nleft)) < 0) {
            if (errno == EINTR) {
                continue;
            } else {
                perror("read");
                exit(EXIT_FAILURE);
            }
        } else if (nread == 0) {
            break; // Connessione chiusa dal peer
        }

        nleft -= nread;
        buf += nread;
    }

    return (count - nleft); // Restituisce il numero totale di byte letti
}