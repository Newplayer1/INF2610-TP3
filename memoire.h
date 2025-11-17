/* memoire.h - Structures et constantes communes pour la gestion de mémoire
 *
 * Utilisé par :
 *   - memoire_template.c (code de départ des étudiants)
 *   - memoire_solution.c (solutionnaire)
 */

#ifndef MEMOIRE_H
#define MEMOIRE_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

/* ---------------- Constantes de configuration ---------------- */

/* Taille d'une page : 2^10 = 1024 octets (comme dans l'énoncé) */
#define TAILLE_PAGE        (1 << 10)

/* On suppose des adresses virtuelles de 16 bits pour simplifier */
#define NB_BITS_ADRESSE    16

/* Nombre de pages virtuelles possibles */
#define NB_PAGES           (1 << (NB_BITS_ADRESSE - 10))  /* 16 - 10 = 6 bits de numéro de page -> 64 pages */

/* Nombre de cadres physiques dans la mémoire principale */
#define NB_CADRES          8

/* Taille du TLB (petit, pour bien voir l'effet du remplacement) */
#define TAILLE_TLB         4

/* Nombre maximal de requêtes simulées */
#define NB_REQUETES_MAX    1000

/* ---------------- Structures de données ---------------- */

typedef struct {
    uint16_t adresse_virtuelle;
    unsigned long instant;
} RequeteMemoire;

typedef struct {
    int page;              /* Numéro de page virtuelle */
    int cadre;             /* Numéro de cadre physique */
    int valide;            /* 0 = invalide, 1 = valide */
    unsigned long last_access; /* Pour les stats / debug (non nécessaire pour FIFO) */
} TLBEntry;

typedef struct {
    int cadre;             /* Numéro de cadre physique */
    int valide;            /* 0 = invalide, 1 = valide */
    unsigned long last_access; /* Pour debug / politiques plus avancées */
} PageTableEntry;

typedef struct {
    TLBEntry tlb[TAILLE_TLB];
    PageTableEntry tp[NB_PAGES];
    int mem_physique[NB_CADRES][TAILLE_PAGE];

    int nb_cadres_utilises;
    unsigned long temps;

    /* Stats pour évaluer l'impact du TLB */
    unsigned long nb_acces_total;
    unsigned long nb_tlb_hits;
    unsigned long nb_defauts_page;
} SystemeMemoire;

/* ---------------- Prototypes des fonctions à implémenter ---------------- */

/* 1. Numéro de page à partir d'une adresse virtuelle */
int calculer_numero_page(uint16_t adresse_virtuelle);

/* 2. Offset dans la page */
int calculer_offset(uint16_t adresse_virtuelle);

/* 3. Construction de l'adresse physique à partir du cadre et de l'offset */
uint16_t construire_adresse_physique(int cadre, int offset);

/* 4. Recherche dans le TLB
 * Retourne 1 si succès (TLB hit, *adr_physique rempli)
 * Retourne 0 si miss
 */
int rechercher_tlb(SystemeMemoire *sys, int page, uint16_t *adr_physique);

/* 5. Recherche dans la table des pages
 * Retourne 1 si la page est présente (*adr_physique rempli)
 * Retourne 0 si défaut de page (page absente)
 */
int rechercher_table_pages(SystemeMemoire *sys, int page, uint16_t *adr_physique);

/* 6. Ajout d'une page en mémoire principale (premier cadre libre)
 * Retourne le numéro de cadre alloué, ou -1 si la mémoire est pleine (dans ce TP on suppose qu'il y a toujours de la place).
 */
int ajouter_page_en_memoire(SystemeMemoire *sys, int page);

/* 7. Mise à jour du TLB suivant une politique FIFO */
void maj_tlb_fifo(SystemeMemoire *sys, int page, int cadre);

/* Fonctions utilitaires (peuvent être fournies aux étudiants) */
void initialiser_systeme(SystemeMemoire *sys);
void afficher_stats(SystemeMemoire *sys);

#endif /* MEMOIRE_H */
