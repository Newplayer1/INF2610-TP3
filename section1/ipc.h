#ifndef IPC_H
#define IPC_H

#include <pthread.h>
#include <semaphore.h>
#include <signal.h>

#define TAILLE_TAMPON 10
#define NB_CLIENTS 3
#define NB_TRAVAILLEURS 4

typedef struct
{
    int requetes[TAILLE_TAMPON];
    int debut;
    int fin;
    int nombre;

    pthread_mutex_t mutex;
    pthread_cond_t cond_plein;
    pthread_cond_t cond_vide;
} Tampon;

void initialiser_tampon(Tampon *t);
void detruire_tampon(Tampon *t);

void *thread_travailleur(void *arg);
void envoyer_requete(int id_client, Tampon *t);
int retirer_requete(Tampon *t);

extern volatile sig_atomic_t stop;

#endif
