#ifndef DEADLOCK_H
#define DEADLOCK_H

#include <pthread.h>

typedef struct
{
    pthread_mutex_t A;
    pthread_mutex_t B;
    pthread_mutex_t C;
} Ressources;

void scenario1(Ressources *r);
void scenario2(Ressources *r);
void scenario_corrige(Ressources *r);

#endif
