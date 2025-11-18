#ifndef ORDONNANCEMENT_H
#define ORDONNANCEMENT_H

#define MAX_TACHES 32

typedef enum
{
    TACHE_INTERACTIVE,
    TACHE_CPU,
    TACHE_PERIODIQUE
} TypeTache;

typedef struct
{
    int id;
    int duree;
    int priorite;
    int periode;
    TypeTache type;
} Tache;

void fcfs(Tache *taches, int n);
void sjf(Tache *taches, int n);
void round_robin(Tache *taches, int n, int quantum);

#endif
