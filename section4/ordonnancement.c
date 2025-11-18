#include "ordonnancement.h"
#include <stdio.h>

/* Étudiants : compléter ces trois algorithmes */

void fcfs(Tache *t, int n)
{
    // TODO
}

void sjf(Tache *t, int n)
{
    // TODO
}

void round_robin(Tache *t, int n, int q)
{
    // TODO
}

int main()
{
    Tache t[4] = {
        {0, 5, 1, 0, TACHE_INTERACTIVE},
        {1, 12, 2, 0, TACHE_CPU},
        {2, 3, 1, 5, TACHE_PERIODIQUE},
        {3, 7, 3, 0, TACHE_CPU}};

    // TODO : appeler et comparer les algorithmes

    return 0;
}
