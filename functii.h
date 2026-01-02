#ifndef FUNCTII_H
#define FUNCTII_H

struct Tren
{
    int id;
    char plecare[100], destinatie[100];
    char ora_plecare, ora_sosire;
    char intarziere_plecare, intarziere_sosire;
};

struct TrainDB {
    struct Tren trenuri[100]; // Un array pentru stocare
    int count;               // Numarul curent de trenuri
    // ... mutex-ul
};

int login(char *docname, char username[], char parola[]);
void mersul_trenurilor(char *docname,char msg[]);
void status_plecari(char *docname, char msg[], int ora, int min);
void status_sosiri(char *docname, char msg[], int ora, int min);
void update_plecare(char *docname, char msg[], char id_cautat[], int intarziere);
void update_sosire(char *docname, char msg[], char id_cautat[], int intarziere);
#endif