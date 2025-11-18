/* memoire.c
 *
 * Gabarit étudiant pour la section 3 : gestion de mémoire virtuelle / TLB.
 * À compléter en suivant les TODOs et les spécifications données dans le sujet du TP.
 */

#include "memoire.h"

/* ---------------- Fonctions à compléter ---------------- */

int calculer_numero_page(uint16_t adr_virt)
{
    // TODO : extraire les bits de poids fort de l’adresse virtuelle (numéro de page)
    return -1;
}

int calculer_offset(uint16_t adr_virt)
{
    // TODO : extraire les bits de poids faible (offset dans la page)
    return -1;
}

uint16_t construire_adresse_physique(int cadre, int offset)
{
    // TODO : reconstituer l’adresse physique à partir du cadre et du déplacement
    return 0;
}

int rechercher_tlb(SystemeMemoire *sys, int page, int *cadre_out)
{
    // TODO : rechercher si la page est présente dans le TLB
    // Si oui, mettre à jour *cadre_out et retourner 1 (hit)
    // Sinon, retourner 0 (miss)
    return 0;
}

int rechercher_table_pages(SystemeMemoire *sys, int page, int *cadre_out)
{
    // TODO : vérifier si la page est présente en mémoire principale (table des pages)
    // Si oui, retourner 1 et stocker le cadre
    // Sinon, retourner 0
    return 0;
}

int ajouter_page_en_memoire(SystemeMemoire *sys, int page)
{
    // TODO : insérer la page dans un cadre libre, mettre à jour la table des pages
    // Attention : pas de remplacement à faire dans cette version
    return -1;
}

void maj_tlb_fifo(SystemeMemoire *sys, int page, int cadre)
{
    // TODO : mettre à jour le TLB avec la page et le cadre
    // Utiliser une politique FIFO approximative basée sur last_use
}

/* ---------------- Fonctions utilitaires fournies ---------------- */

void initialiser_systeme(SystemeMemoire *sys)
{
    sys->nb_cadres_utilises = 0;
    sys->temps = 0;
    sys->nb_acces = 0;
    sys->nb_tlb_hits = 0;
    sys->nb_defauts_page = 0;

    for (int i = 0; i < TAILLE_TLB; ++i)
    {
        sys->tlb[i].page = -1;
        sys->tlb[i].cadre = -1;
        sys->tlb[i].valide = 0;
        sys->tlb[i].last_use = 0;
    }

    for (int p = 0; p < NB_PAGES; ++p)
    {
        sys->tp[p].cadre = -1;
        sys->tp[p].valide = 0;
        sys->tp[p].last_use = 0;
    }

    for (int c = 0; c < NB_CADRES; ++c)
    {
        for (int i = 0; i < TAILLE_PAGE; ++i)
        {
            sys->mem_physique[c][i] = 0;
        }
    }
}

void afficher_stats(SystemeMemoire *sys)
{
    printf("\n===== Statistiques mémoire =====\n");
    printf("Nombre total d'accès    : %lu\n", sys->nb_acces);
    printf("TLB hits                 : %lu\n", sys->nb_tlb_hits);
    printf("Défauts de page          : %lu\n", sys->nb_defauts_page);

    if (sys->nb_acces > 0)
    {
        double taux_tlb = (double)sys->nb_tlb_hits / sys->nb_acces * 100.0;
        double taux_def = (double)sys->nb_defauts_page / sys->nb_acces * 100.0;
        printf("Taux de succès TLB       : %.2f %%\n", taux_tlb);
        printf("Taux de défauts de page  : %.2f %%\n", taux_def);
    }
}

/* ---------------- Programme de test de base ---------------- */

int main(void)
{
    SystemeMemoire sys;
    initialiser_systeme(&sys);

    const int nb_requetes = 10;
    uint16_t adresses[nb_requetes];

    for (int i = 0; i < nb_requetes; ++i)
    {
        adresses[i] = (uint16_t)(rand() % (1 << NB_BITS_ADR));
    }

    for (int i = 0; i < nb_requetes; ++i)
    {
        sys.temps++;
        sys.nb_acces++;

        uint16_t adr_virt = adresses[i];
        int page = calculer_numero_page(adr_virt);
        int offset = calculer_offset(adr_virt);
        int cadre = -1;
        int tlb_hit = 0;

        if (rechercher_tlb(&sys, page, &cadre))
        {
            tlb_hit = 1;
            sys.nb_tlb_hits++;
        }
        else
        {
            if (!rechercher_table_pages(&sys, page, &cadre))
            {
                sys.nb_defauts_page++;
                cadre = ajouter_page_en_memoire(&sys, page);
            }
            maj_tlb_fifo(&sys, page, cadre);
        }

        uint16_t adr_phys = construire_adresse_physique(cadre, offset);

        printf("Accès %2d : VA = 0x%04x (page=%2d, offset=%4d) -> PA = 0x%04x%s\n",
               i, adr_virt, page, offset, adr_phys,
               tlb_hit ? " [TLB]" : "");
    }

    afficher_stats(&sys);
    return 0;
}
