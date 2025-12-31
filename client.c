/* cliTCPIt.c - Exemplu de client TCP
   Trimite un numar la server; primeste de la server numarul incrementat.

   Autor: Lenuta Alboaie  <adria@info.uaic.ro> (c)
*/
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include <string.h>

/* codul de eroare returnat de anumite apeluri */
extern int errno;

/* portul de conectare la server*/
int port;

int comanda_valida(char buf[]);

int main(int argc, char *argv[])
{
    int sd;                    // descriptorul de socket
    struct sockaddr_in server; // structura folosita pentru conectare
                               // mesajul trimis
    int nr = 0, bytes;
    char buf[1000];

    /* exista toate argumentele in linia de comanda? */
    if (argc != 3)
    {
        printf("Sintaxa: %s <adresa_server> <port>\n", argv[0]);
        return -1;
    }

    /* stabilim portul */
    port = atoi(argv[2]);

    /* cream socketul */
    if ((sd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        perror("Eroare la socket().\n");
        return errno;
    }

    /* umplem structura folosita pentru realizarea conexiunii cu serverul */
    /* familia socket-ului */
    server.sin_family = AF_INET;
    /* adresa IP a serverului */
    server.sin_addr.s_addr = inet_addr(argv[1]);
    /* portul de conectare */
    server.sin_port = htons(port);

    /* ne conectam la server */
    if (connect(sd, (struct sockaddr *)&server, sizeof(struct sockaddr)) == -1)
    {
        perror("[client]Eroare la connect().\n");
        return errno;
    }

    while (1)
    { /* citirea mesajului */
        printf("[client]Introduceti o comanda: ");
        fflush(stdout);

        bzero(buf, sizeof(buf));
        read(0, buf, sizeof(buf));

        /* trimiterea mesajului la server */

        buf[strlen(buf) - 1] = '\0';

        bytes = strlen(buf);

        if (write(sd, &bytes, sizeof(int)) <= 0)
        {
            perror("[client]Eroare la write() spre server.\n");
            return errno;
        }

        if (write(sd, buf, bytes) <= 0)
        {
            perror("[client]Eroare la write() spre server.\n");
            return errno;
        }
        /* citirea raspunsului dat de server
           (apel blocant pina cind serverul raspunde) */

        if (strcmp(buf, "quit") == 0)
        {
            close(sd);
            return 0;
        }
        
        if (read(sd, &bytes, sizeof(int)) < 0)
        {
            perror("[client]Eroare la read() de la server.\n");
            return errno;
        }

        char ch;
        for (int i = 0; i < bytes; i++)
        {
            if (read(sd, &ch, sizeof(char)) < 0)
            {
                perror("[client]Eroare la read() de la server.\n");
                return errno;
            }
            printf("%c", ch);
        }
        /* afisam mesajul primit */
        // printf("[client]Mesajul primit este: %d\n", nr);

        /* inchidem conexiunea, am terminat */
    }
}
