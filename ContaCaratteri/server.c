#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <errno.h>
#include <string.h>
#include <sys/select.h>

ssize_t FullWrite(int fd, const void *buf, size_t count);
ssize_t FullRead(int fd, void *buf, size_t count);

#define BACKLOG 10
#define MAXLINE 256


int main(int argc, char *argv[])
{

    int waiting = 0;
    int compat = 0;
    struct sockaddr_in s_addr, c_addr;
    socklen_t len;
    char buffer[MAXLINE];
    char fd_open[FD_SETSIZE];
    fd_set fset;
    int list_fd, fd;
    int max_fd, nread, nwrite;
    int i, n;

    if ((list_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0)               /* list_fd = 3 */
    { 
        perror("Socket creation error");
        exit(1);
    }


    memset((void )&s_addr, 0, sizeof(s_addr));                      / clear server address */
    s_addr.sin_family = AF_INET;                                     /* address type is INET */
    s_addr.sin_port = htons(1024);                                   /* echo port is 7 */
    s_addr.sin_addr.s_addr = htonl(INADDR_ANY);                      /* connect from anywhere */

    
    if (bind(list_fd, (struct sockaddr *)&s_addr, sizeof(s_addr)) < 0)
    {
        perror("bind error");
        exit(1);
    }

    
    if (listen(list_fd, BACKLOG) < 0)
    {
        perror("listen error");
        exit(1);
    }

   
    memset(fd_open, 0, FD_SETSIZE);                                     /* clear array of open files */
    max_fd = list_fd;                                                   /* maximum now is listening socket (3)*/
    fd_open[max_fd] = 1;                                                /* è ilsecondo array  0 0 0 1 ...*/

    
    while (1)
    {
        FD_ZERO(&fset);
        for (i = list_fd; i <= max_fd; i++)
        { 
            /* initialize fd_set */
            if (fd_open[i] != 0)
                FD_SET(i, &fset);                                      /* pone a 3 l'fset è il primo array*/
        }

        while (((n = select(max_fd, &fset, NULL, NULL, NULL )) < 0) && (errno == EINTR));   /* wait for data or connection */
        if (n < 0)
        {   
            /* on real error exit */
            perror("select error");
            exit(1);
        }

        /* on activity */
        if (FD_ISSET(list_fd, &fset))   /* if new connection */
        {       
            n--;                        /* decrement active (sono le connessioni che stiamo gestendo  es. se in fd_open ci sono due 1 allora n = 2 perché sono due connessioni) */
            len = sizeof(c_addr);

            if ((fd = accept(list_fd, (struct sockaddr )&c_addr, &len)) < 0)            / accept new connection */
            {
                perror("accept error");
                exit(1);
            }

            fd_open[fd] = 1;        /* set new connection socket */
            if (max_fd < fd)
                max_fd = fd;        /* if needed set new maximum */
        }

        /* loop on open connections */
        i = list_fd;                    /* first socket to look */
        while (n != 0)                  /* loop until active */
        {        
            i++;                        /* start after listening socket */
            if (fd_open[i] == 0)
                continue;               /* closed, go next */
            if (FD_ISSET(i, &fset))
            {
                n--;                                                            /* decrease active */
                nread = FullRead(i, buffer, sizeof(buffer));                    /* read operations */
                if (nread < 0)
                {
                    perror("Errore in lettura");
                    exit(1);
                }
                if (nread == 0)
                {                                    /* if closed connection */
                    close(list_fd);                  /* close file */
                    fd_open[i] = 0;                  /* mark as closed in table */
                    if (max_fd == i)
                    { 
                        /* if was the maximum */
                        while (fd_open[--i] == 0);       /* loop down */
                        max_fd = i;                      /* set new maximum */
                        break;                           /* and go back to select */
                    }
                    continue; /* continue loop on open */
                }

                buffer[nread] = 0;

                // Calcola il numero di caratteri nella stringa ricevuta
                int charCount = strlen(buffer);

                // Invia il numero di caratteri al client
                char response[100];
                snprintf(response, sizeof(response), "Numero di caratteri nella stringa: %d\n", charCount);
                FullWrite(fd, response, strlen(response));

                close(fd);

                if (nwrite)
                {
                    perror("Errore in scrittura");
                    exit(1);
                }
            }
        }
    }
    /* normal exit, never reached */
    exit(0);
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