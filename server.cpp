/* servTCPConcTh2.c - Exemplu de server TCP concurent care deserveste clientii
   Asteapta un numar de la clienti si intoarce clientilor numarul incrementat.
    Intoarce corect identificatorul din program al thread-ului.


   Autor: Lenuta Alboaie  <adria@info.uaic.ro> (c)
*/

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>
#include <unistd.h>
#include <stdio.h>
#include <cstring>
#include <stdlib.h>
#include <signal.h>
#include <pthread.h>
#include <time.h>
#include <queue>
#include "functii.h"

using namespace std;

/* portul folosit */
#define PORT 2908

/* codul de eroare returnat de anumite apeluri */
extern int errno;

typedef struct thData
{
    int idThread; // id-ul thread-ului tinut in evidenta de acest program
    int cl;       // descriptorul intors de accept
} thData;

static void *treat(void *); /* functia executata de fiecare thread ce realizeaza comunicarea cu clientii */
static void *thread_cq(void *);
void raspunde(void *);
__thread int este_logat = 0;
pthread_mutex_t lock;
pthread_rwlock_t rwlock;

int main()
{
    struct sockaddr_in server; // structura folosita de server
    struct sockaddr_in from;
    int sd; // descriptorul de socket
    pthread_t th[100]; // Identificatorii thread-urilor care se vor crea
    int i = 0;

    /* crearea unui socket */
    if ((sd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        perror("[server]Eroare la socket().\n");
        return errno;
    }
    /* utilizarea optiunii SO_REUSEADDR */
    int on = 1;
    setsockopt(sd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));

    /* pregatirea structurilor de date */
    bzero(&server, sizeof(server));
    bzero(&from, sizeof(from));

    /* umplem structura folosita de server */
    /* stabilirea familiei de socket-uri */
    server.sin_family = AF_INET;
    /* acceptam orice adresa */
    server.sin_addr.s_addr = htonl(INADDR_ANY);
    /* utilizam un port utilizator */
    server.sin_port = htons(PORT);

    /* atasam socketul */
    if (bind(sd, (struct sockaddr *)&server, sizeof(struct sockaddr)) == -1)
    {
        perror("[server]Eroare la bind().\n");
        return errno;
    }

    /* punem serverul sa asculte daca vin clienti sa se conecteze */
    if (listen(sd, 2) == -1)
    {
        perror("[server]Eroare la listen().\n");
        return errno;
    }
    /* servim in mod concurent clientii...folosind thread-uri */

    pthread_t tid1,tid2;
    pthread_mutex_init(&lock, NULL);
    pthread_rwlock_init(&rwlock, NULL);

    pthread_create(&tid1, NULL, &thread_cq, NULL);
    pthread_create(&tid2, NULL, &thread_cq, NULL);

    while (1)
    {
        int client;
        thData *td; // parametru functia executata de thread
        unsigned int length = sizeof(from);

        printf("[server]Asteptam la portul %d...\n", PORT);
        fflush(stdout);

        // client= malloc(sizeof(int));
        /* acceptam un client (stare blocanta pina la realizarea conexiunii) */
        if ((client = accept(sd, (struct sockaddr *)&from, &length)) < 0)
        {
            perror("[server]Eroare la accept().\n");
            continue;
        }

        /* s-a realizat conexiunea, se astepta mesajul */

        // int idThread; //id-ul threadului
        // int cl; //descriptorul intors de accept

        td = (struct thData *)malloc(sizeof(struct thData));
        td->idThread = i++;
        td->cl = client;

        pthread_create(&th[i], NULL, &treat, td);

    } // while
};

typedef struct qelem
{
    int client;
    char comanda[100];
    int tid;
    int este_logat;
} qelem;

queue<qelem> coada;

void login_comanda(char username[], char parola[]);
void logout();
char msg[100000], buf[100], ch, text[20], username[10], parola[10], id[10];
int intarziere;

static void *thread_cq(void*)
{
    pthread_detach(pthread_self());

    while (1)
    {
        pthread_mutex_lock(&lock);
        if (coada.empty())
        {
            pthread_mutex_unlock(&lock);
            sleep(1);
            continue;
        }
        qelem c = coada.front();
        coada.pop();
        pthread_mutex_unlock(&lock);

        bzero(msg, sizeof(msg));
        bzero(text, sizeof(text));
        bzero(username, sizeof(username));
        bzero(parola, sizeof(parola));
        bzero(id, sizeof(id));

        /*pregatim mesajul de raspuns */

        if (strcmp(c.comanda, "status_plecari") == 0)
        {
            if (c.este_logat)
            {
                time_t now = time(NULL);
                struct tm *t = localtime(&now);

                pthread_rwlock_rdlock(&rwlock);
                status_plecari((char *)"trenuri.xml", msg, t->tm_hour, t->tm_min);
                pthread_rwlock_unlock(&rwlock);
            }
            else
                sprintf(msg, "Trebuie sa va logati mai intai!\n");
        }
        else if (strcmp(c.comanda, "status_sosiri") == 0)
        {
            if (c.este_logat)
            {
                time_t now = time(NULL);
                struct tm *t = localtime(&now);

                pthread_rwlock_rdlock(&rwlock);
                status_sosiri((char *)"trenuri.xml", msg, t->tm_hour, t->tm_min);
                pthread_rwlock_unlock(&rwlock);
            }
            else
                sprintf(msg, "Trebuie sa va logati mai intai!\n");
        }
        else if (strcmp(c.comanda, "mersul_trenurilor") == 0)
        {
            if (c.este_logat)
            {
                pthread_rwlock_rdlock(&rwlock);
                mersul_trenurilor((char *)"trenuri.xml", msg);
                pthread_rwlock_unlock(&rwlock);
            }
            else
                sprintf(msg, "Trebuie sa va logati mai intai!\n");
        }
        else if (strncmp(c.comanda, "update_plecare", strlen("update_plecare")) == 0)
        {
            if (c.este_logat)
            {
                sscanf(c.comanda, "%s %s %d", text, id, &intarziere);

                pthread_rwlock_wrlock(&rwlock);
                update_plecare((char *)"trenuri.xml", msg, id, intarziere);
                pthread_rwlock_unlock(&rwlock);
            }
            else
                sprintf(msg, "Trebuie sa va logati mai intai!\n");
        }
        else
        {
            if (c.este_logat)
            {
                sscanf(c.comanda, "%s %s %d", text, id, &intarziere);

                pthread_rwlock_wrlock(&rwlock);
                update_sosire((char *)"trenuri.xml", msg, id, intarziere);
                pthread_rwlock_unlock(&rwlock);
            }
            else
                sprintf(msg, "Trebuie sa va logati mai intai!\n");
        }

        int bytes = strlen(msg);

        if (write(c.client, &bytes, sizeof(int)) <= 0)
        {
            printf("[Thread %d] ", c.tid);
            perror("[Thread]Eroare la write() catre client.\n");
        }
        else
            printf("[Thread %d]Mesajul a fost trasmis cu succes.\n", c.tid);

        if (write(c.client, msg, strlen(msg)) <= 0)
        {
            printf("[Thread %d] ", c.tid);
            perror("[Thread]Eroare la write() catre client.\n");
        }
        else
            printf("[Thread %d]Mesajul a fost trasmis cu succes.\n", c.tid);
    }

    return (NULL);
};

static void *treat(void *arg)
{
    // struct thData tdL;
    struct thData *tdL = (struct thData *)arg;
    // tdL = *((struct thData *)arg);
    printf("[thread]- %d - Asteptam mesajul...\n", tdL->idThread);
    fflush(stdout);
    pthread_detach(pthread_self());
    raspunde(tdL);
    /* am terminat cu acest client, inchidem conexiunea */
    close(tdL->cl);
    free(tdL);
    return (NULL);
};

int comanda_valida(char buf[])
{
    if (strcmp(buf, "mersul_trenurilor") == 0 || strcmp(buf, "status_plecari") == 0 || strcmp(buf, "status_sosiri") == 0 || strncmp(buf, "logout", strlen("logout")) == 0 || strcmp(buf, "quit") == 0)
        return 1;

    if (strncmp(buf, "update_plecare", strlen("update_plecare")) == 0)
    {
        bzero(text, sizeof(text));
        if (sscanf(buf, "%s %s %d", text, id, &intarziere) != 3)
            return 0;
        int id_int = 0;
        sscanf(id, "%d", &id_int);
        if (id_int < 0 || intarziere < 0)
            return 0;
        return 1;
    }

    if (strncmp(buf, "update_sosire", strlen("update_sosire")) == 0)
    {
        bzero(text, sizeof(text));
        if (sscanf(buf, "%s %s %d", text, id, &intarziere) != 3)
            return 0;
        int id_int = 0;
        sscanf(id, "%d", &id_int);
        if (id_int < 0)
            return 0;
        return 1;
    }

    if (strncmp(buf, "login", strlen("login")) == 0)
    {
        bzero(text, sizeof(text));
        bzero(username, sizeof(username));
        bzero(parola, sizeof(parola));
        if (sscanf(buf, "%s %s %s", text, username, parola) != 3)
            return 0;
        return 1;
    }

    return 0;
}

void raspunde(void *arg)
{
    int bytes;
    struct thData *tdL = (struct thData *)arg;
    while (1)
    {
        if (read(tdL->cl, &bytes, sizeof(int)) <= 0)
        {
            printf("[Thread %d]\n", tdL->idThread);
            perror("Eroare la read() de la client.\n");
        }

        bzero(buf, sizeof(buf));

        if (read(tdL->cl, buf, bytes) <= 0)
        {
            printf("[Thread %d]\n", tdL->idThread);
            perror("Eroare la read() de la client.\n");
        }
        printf("[Thread %d]Mesajul a fost receptionat...%s\n", tdL->idThread, buf);

        if (!comanda_valida(buf))
        {
            strcpy(msg, "Comanda invalida!\n");
            bytes = strlen(msg);

            if (write(tdL->cl, &bytes, sizeof(int)) <= 0)
            {
                printf("[Thread %d] ", tdL->idThread);
                perror("[Thread]Eroare la write() catre client.\n");
            }
            else
                printf("[Thread %d]Mesajul a fost trasmis cu succes.\n", tdL->idThread);

            if (write(tdL->cl, msg, strlen(msg)) <= 0)
            {
                printf("[Thread %d] ", tdL->idThread);
                perror("[Thread]Eroare la write() catre client.\n");
            }
            else
                printf("[Thread %d]Mesajul a fost trasmis cu succes.\n", tdL->idThread);
        }
        else
        {
            if (strcmp(buf, "quit") == 0)
            {
                return;
            }
            else if (strncmp(buf, "login", strlen("login")) == 0)
            {
                sscanf(buf, "%s %s %s", text, username, parola);
                login_comanda(username, parola);
                bytes = strlen(msg);

                if (write(tdL->cl, &bytes, sizeof(int)) <= 0)
                {
                    printf("[Thread %d] ", tdL->idThread);
                    perror("[Thread]Eroare la write() catre client.\n");
                }
                else
                    printf("[Thread %d]Mesajul a fost trasmis cu succes.\n", tdL->idThread);

                if (write(tdL->cl, msg, strlen(msg)) <= 0)
                {
                    printf("[Thread %d] ", tdL->idThread);
                    perror("[Thread]Eroare la write() catre client.\n");
                }
                else
                    printf("[Thread %d]Mesajul a fost trasmis cu succes.\n", tdL->idThread);
            }
            else if (strcmp(buf, "logout") == 0)
            {
                logout();
                bytes = strlen(msg);

                if (write(tdL->cl, &bytes, sizeof(int)) <= 0)
                {
                    printf("[Thread %d] ", tdL->idThread);
                    perror("[Thread]Eroare la write() catre client.\n");
                }
                else
                    printf("[Thread %d]Mesajul a fost trasmis cu succes.\n", tdL->idThread);

                if (write(tdL->cl, msg, strlen(msg)) <= 0)
                {
                    printf("[Thread %d] ", tdL->idThread);
                    perror("[Thread]Eroare la write() catre client.\n");
                }
                else
                    printf("[Thread %d]Mesajul a fost trasmis cu succes.\n", tdL->idThread);
            }
            else
            {
                qelem c;
                c.client = tdL->cl;
                strcpy(c.comanda, buf);
                c.tid = tdL->idThread;
                c.este_logat = este_logat;
                pthread_mutex_lock(&lock);
                coada.push(c);
                pthread_mutex_unlock(&lock);
            }
        }
    }
}

void login_comanda(char username[], char parola[])
{
    if (este_logat)
    {
        sprintf(msg, "Sunteti deja logat!\n");
        return;
    }
    int x = login((char *)"log.xml", username, parola);
    if (x == 1)
    {
        sprintf(msg, "V-ati logat cu succes!\n");
        este_logat = 1;
    }
    else
        sprintf(msg, "Username sau parola incorecte!\n");
}

void logout()
{
    if (este_logat)
    {
        este_logat = 0;
        sprintf(msg, "V-ati delogat cu succes!\n");
    }
    else
        sprintf(msg, "Nu sunteti logat!\n");
}

/* returnam mesajul clientului */
