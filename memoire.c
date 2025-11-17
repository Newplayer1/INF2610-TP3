/* memoire_template.c
 *
 * Point de départ pour la section "Gestion de la mémoire virtuelle" du TP.
 *
 * Complétez les fonctions marquées TODO. Ne modifiez pas les signatures
 * des fonctions, afin de faciliter la correction automatique.
 */

#include "memoire.h"

/* ---------------- Fonctions à compléter ---------------- */

int calculer_numero_page(uint16_t adresse_virtuelle)
{
    /* TODO : extraire le numéro de page à partir de l'adresse virtuelle.
     * Indice : utilisez un décalage vers la droite.
     */
    (void)adresse_virtuelle; /* pour éviter un warning si non utilisé */
    return -1;               /* valeur temporaire, à remplacer */
}

int calculer_offset(uint16_t adresse_virtuelle)
{
    /* TODO : extraire l'offset (déplacement dans la page).
     * Indice : utilisez un masque (modulo TAILLE_PAGE).
     */
    (void)adresse_virtuelle;
    return -1; /* valeur temporaire, à remplacer */
}

uint16_t construire_adresse_physique(int cadre, int offset)
{
    /* TODO : construire l'adresse physique à partir du numéro de cadre
     * et de l'offset. Le cadre joue le même rôle que le numéro de page
     * dans l'adresse virtuelle.
     */
    (void)cadre;
    (void)offset;
    return 0; /* valeur temporaire, à remplacer */
}

int rechercher_tlb(SystemeMemoire *sys, int page, uint16_t *adr_physique)
{
    /* TODO :
     *  - Parcourir le TLB.
     *  - Si une entrée valide contient la page demandée :
     *      - mettre à jour last_access (optionnel pour FIFO, utile pour debug)
     *      - construire l'adresse physique (cadre + offset sera ajouté plus tard)
     *      - retourner 1
     *  - Sinon, retourner 0
     *
     * Attention : cette fonction ne gère que la traduction page->cadre.
     * L'offset sera ajouté ensuite par l'appelant.
     */

    (void)sys;
    (void)page;
    (void)adr_physique;
    return 0; /* miss par défaut */
}

int rechercher_table_pages(SystemeMemoire *sys, int page, uint16_t *adr_physique)
{
    /* TODO :
     *  - Vérifier si la page est valide dans la table des pages.
     *  - Si oui, construire l'adresse physique (cadre + offset sera ajouté plus tard)
     *    et retourner 1.
     *  - Sinon, retourner 0 (défaut de page).
     */

    (void)sys;
    (void)page;
    (void)adr_physique;
    return 0; /* défaut de page par défaut */
}

int ajouter_page_en_memoire(SystemeMemoire *sys, int page)
{
    /* TODO :
     *  - Trouver le premier cadre libre (par exemple en utilisant sys->nb_cadres_utilises).
     *  - Marquer l'entrée correspondante dans la table des pages comme valide.
     *  - Mettre à jour les champs (cadre, last_access, etc.).
     *  - Incrémenter le compteur de cadres utilisés.
     *  - Retourner le numéro de cadre.
     *
     * On suppose qu'il y a toujours un cadre libre (pas de remplacement).
     */

    (void)sys;
    (void)page;
    return -1; /* à remplacer */
}

void maj_tlb_fifo(SystemeMemoire *sys, int page, int cadre)
{
    /* TODO :
     *  - Implémenter une politique FIFO sur le TLB.
     *    Idée simple : utilisez sys->temps comme compteur global, et choisissez
     *    soit la première case libre, soit l'entrée avec le plus petit last_access.
     *  - Mettre à jour l'entrée choisie (page, cadre, valide=1, last_access=temps courant).
     */

    (void)sys;
    (void)page;
    (void)cadre;
}

/* ---------------- Fonctions utilitaires fournies ---------------- */

void initialiser_systeme(SystemeMemoire *sys)
{
    sys->nb_cadres_utilises = 0;
    sys->temps = 0;
    sys->nb_acces_total = 0;
    sys->nb_tlb_hits = 0;
    sys->nb_defauts_page = 0;

    for (int i = 0; i < TAILLE_TLB; ++i)
    {
        sys->tlb[i].page = -1;
        sys->tlb[i].cadre = -1;
        sys->tlb[i].valide = 0;
        sys->tlb[i].last_access = 0;
    }

    for (int p = 0; p < NB_PAGES; ++p)
    {
        sys->tp[p].cadre = -1;
        sys->tp[p].valide = 0;
        sys->tp[p].last_access = 0;
    }
}

void afficher_stats(SystemeMemoire *sys)
{
    printf("===== Statistiques mémoire =====\n");
    printf("Nombre total d'accès    : %lu\n", sys->nb_acces_total);
    printf("TLB hits                 : %lu\n", sys->nb_tlb_hits);
    printf("Défauts de page          : %lu\n", sys->nb_defauts_page);

    if (sys->nb_acces_total > 0)
    {
        double taux_tlb = (double)sys->nb_tlb_hits / sys->nb_acces_total * 100.0;
        double taux_defauts = (double)sys->nb_defauts_page / sys->nb_acces_total * 100.0;
        printf("Taux de succès TLB       : %.2f %%\n", taux_tlb);
        printf("Taux de défauts de page  : %.2f %%\n", taux_defauts);
    }
}

/* ---------------- Programme de test / simulation ---------------- */

int main(void)
{
    SystemeMemoire sys;
    initialiser_systeme(&sys);

    /* Pour simplifier : on génère quelques adresses virtuelles pseudo-aléatoires.
     * Dans ta version finale, tu peux lire les adresses depuis un fichier ou stdin.
     */
    const int nb_requetes = 20;
    uint16_t adresses[nb_requetes];

    for (int i = 0; i < nb_requetes; ++i)
    {
        adresses[i] = (uint16_t)(rand() % (1 << NB_BITS_ADRESSE));
    }

    for (int i = 0; i < nb_requetes; ++i)
    {
        uint16_t adr_virt = adresses[i];
        int page = calculer_numero_page(adr_virt);
        int offset = calculer_offset(adr_virt);
        uint16_t adr_physique = 0;
        int tlb_hit = 0;

        sys.temps++;
        sys.nb_acces_total++;

        /* 1. On cherche d'abord dans le TLB */
        tlb_hit = rechercher_tlb(&sys, page, &adr_physique);

        if (tlb_hit)
        {
            sys.nb_tlb_hits++;
            /* On ajoute maintenant l'offset pour obtenir l'adresse physique finale */
            int cadre = calculer_numero_page(adr_physique); /* ou stocker le cadre séparément */
            adr_physique = construire_adresse_physique(cadre, offset);
        }
        else
        {
            /* 2. Sinon, on cherche dans la table des pages */
            int trouve = rechercher_table_pages(&sys, page, &adr_physique);

            if (!trouve)
            {
                /* 3. Défaut de page : on charge la page en mémoire */
                sys.nb_defauts_page++;
                int cadre = ajouter_page_en_memoire(&sys, page);

                /* 4. Mise à jour du TLB */
                maj_tlb_fifo(&sys, page, cadre);

                /* 5. Construction de l'adresse physique finale */
                adr_physique = construire_adresse_physique(cadre, offset);
            }
            else
            {
                /* Page trouvée en mémoire : on peut aussi mettre à jour le TLB */
                int cadre = calculer_numero_page(adr_physique);
                maj_tlb_fifo(&sys, page, cadre);
                adr_physique = construire_adresse_physique(cadre, offset);
            }
        }

        printf("Accès %2d : VA = 0x%04x (page=%d, offset=%d) -> PA = 0x%04x\n",
               i, adr_virt, page, offset, adr_physique);
    }

    afficher_stats(&sys);
    return 0;
}
