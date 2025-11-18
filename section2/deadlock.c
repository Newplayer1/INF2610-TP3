#include "deadlock.h"
#include <stdio.h>
#include <unistd.h>

/* ---------------- Threads pour SCÉNARIO 1 ---------------- */
/* Deux threads, deux verrous, ordre inversé -> enlacement circulaire classique */

typedef struct
{
    Ressources *r;
    int id;
} ArgThread;

static void *s1_thread1(void *arg)
{
    Ressources *r = ((ArgThread *)arg)->r;
    printf("[S1-T1] Lock A\n");
    pthread_mutex_lock(&r->A);
    sleep(1);
    printf("[S1-T1] Lock B (peut bloquer...)\n");
    pthread_mutex_lock(&r->B);

    printf("[S1-T1] Section critique A,B\n");
    pthread_mutex_unlock(&r->B);
    pthread_mutex_unlock(&r->A);
    return NULL;
}

static void *s1_thread2(void *arg)
{
    Ressources *r = ((ArgThread *)arg)->r;
    printf("[S1-T2] Lock B\n");
    pthread_mutex_lock(&r->B);
    sleep(1);
    printf("[S1-T2] Lock A (peut bloquer...)\n");
    pthread_mutex_lock(&r->A);

    printf("[S1-T2] Section critique B,A\n");
    pthread_mutex_unlock(&r->A);
    pthread_mutex_unlock(&r->B);
    return NULL;
}

void scenario1(Ressources *r)
{
    printf("=== SCÉNARIO 1 : Deadlock possible (A puis B / B puis A) ===\n");
    pthread_t t1, t2;
    ArgThread a1 = {.r = r, .id = 1};
    ArgThread a2 = {.r = r, .id = 2};

    pthread_create(&t1, NULL, s1_thread1, &a1);
    pthread_create(&t2, NULL, s1_thread2, &a2);

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);
}

/* ---------------- Threads pour SCÉNARIO 2 ---------------- */
/* Trois threads, trois verrous, cycle A->B, B->C, C->A */

static void *s2_thread1(void *arg)
{
    Ressources *r = ((ArgThread *)arg)->r;
    printf("[S2-T1] Lock A\n");
    pthread_mutex_lock(&r->A);
    sleep(1);
    printf("[S2-T1] Lock B (peut bloquer...)\n");
    pthread_mutex_lock(&r->B);

    printf("[S2-T1] Section critique A,B\n");
    pthread_mutex_unlock(&r->B);
    pthread_mutex_unlock(&r->A);
    return NULL;
}

static void *s2_thread2(void *arg)
{
    Ressources *r = ((ArgThread *)arg)->r;
    printf("[S2-T2] Lock B\n");
    pthread_mutex_lock(&r->B);
    sleep(1);
    printf("[S2-T2] Lock C (peut bloquer...)\n");
    pthread_mutex_lock(&r->C);

    printf("[S2-T2] Section critique B,C\n");
    pthread_mutex_unlock(&r->C);
    pthread_mutex_unlock(&r->B);
    return NULL;
}

static void *s2_thread3(void *arg)
{
    Ressources *r = ((ArgThread *)arg)->r;
    printf("[S2-T3] Lock C\n");
    pthread_mutex_lock(&r->C);
    sleep(1);
    printf("[S2-T3] Lock A (peut bloquer...)\n");
    pthread_mutex_lock(&r->A);

    printf("[S2-T3] Section critique C,A\n");
    pthread_mutex_unlock(&r->A);
    pthread_mutex_unlock(&r->C);
    return NULL;
}

void scenario2(Ressources *r)
{
    printf("=== SCÉNARIO 2 : Deadlock possible (cycle A->B->C->A) ===\n");
    pthread_t t1, t2, t3;
    ArgThread a = {.r = r, .id = 0};

    pthread_create(&t1, NULL, s2_thread1, &a);
    pthread_create(&t2, NULL, s2_thread2, &a);
    pthread_create(&t3, NULL, s2_thread3, &a);

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);
    pthread_join(t3, NULL);
}

void scenario_corrige(Ressources *r)
{
    // TODO : la seule fonction que l'étudiant doit MODIFIER
}

int main()
{
    Ressources r;
    pthread_mutex_init(&r.A, NULL);
    pthread_mutex_init(&r.B, NULL);
    pthread_mutex_init(&r.C, NULL);

    // TODO : appeler les scénarios

    pthread_mutex_destroy(&r.A);
    pthread_mutex_destroy(&r.B);
    pthread_mutex_destroy(&r.C);

    return 0;
}
