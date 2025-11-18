#include "ipc.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

volatile sig_atomic_t stop = 0;

void arreter(int sig)
{
    stop = 1;
}

void initialiser_tampon(Tampon *t)
{
    // TODO
}

void detruire_tampon(Tampon *t)
{
    // TODO
}

void envoyer_requete(int id_client, Tampon *t)
{
    // TODO : mutex + cond
}

int retirer_requete(Tampon *t)
{
    // TODO
    return -1;
}

void *thread_travailleur(void *arg)
{
    // TODO : retirer requête, traiter, afficher
    return NULL;
}

int main(void)
{
    signal(SIGINT, arreter);

    Tampon t;
    initialiser_tampon(&t);

    pthread_t travailleurs[NB_TRAVAILLEURS];
    for (int i = 0; i < NB_TRAVAILLEURS; ++i)
        pthread_create(&travailleurs[i], NULL, thread_travailleur, NULL);

    // TODO : créer clients (simples boucles)

    stop = 1;

    for (int i = 0; i < NB_TRAVAILLEURS; ++i)
        pthread_join(travailleurs[i], NULL);

    detruire_tampon(&t);
}
