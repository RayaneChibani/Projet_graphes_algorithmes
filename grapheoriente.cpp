#include "grapheOriente.h"

grapheOriente::grapheOriente(std::vector<std::vector<int>> matADJ, std::vector<sommet> sommets) : graphe(matADJ, sommets)
{
    //det_dde_ddi();
    //mettreAjourLesIdDesSommets();
}

grapheOriente::grapheOriente(std::vector<int> fs, std::vector<int> aps, std::vector<int> couts, std::vector<sommet> sommets) : graphe(fs, aps, couts, sommets)
{
    det_degres();
    mettreAjourLesIdDesSommets();
}

grapheOriente::grapheOriente (std::string nomDuFichier): graphe(nomDuFichier)
{
    det_degres();
}

grapheOriente::grapheOriente(liste_Principale_Secondaire* listePrincipaleSecondaire, std::vector<sommet> sommets): graphe(listePrincipaleSecondaire,sommets)
{
    det_degres();
    mettreAjourLesIdDesSommets();
}

void grapheOriente::ajouterLien(lien l)
{
    //j'ajoute un lien que quand ya pas de lien existant
    if(matADJ[l.a.id-1][l.b.id-1]==PAS_DE_LIEN)
    {
        matADJ[l.a.id-1][l.b.id-1]=l.poids;
        this->m++;
        dde[l.a.id-1]++;
        ddi[l.b.id-1]++;
    }
}

void grapheOriente::supprimerLien(lien l)
{
    //je supprime un lien que quand le lien existe
    if(matADJ[l.a.id-1][l.b.id-1]!=PAS_DE_LIEN)
    {
        matADJ[l.a.id-1][l.b.id-1]=PAS_DE_LIEN;
        this->m--;
        dde[l.a.id-1]--;
        ddi[l.b.id-1]--;
    }
}

void grapheOriente::setPoidsDe(lien l)
{
    //je modifie le poids d'un lien que quand le lien existe
    if(matADJ[l.a.id-1][l.b.id-1]!=PAS_DE_LIEN){
        matADJ[l.a.id-1][l.b.id-1]=l.poids;
    }
}

int grapheOriente::detDdeDuSommet(int id) const
{
    return dde[id-1];
}

int grapheOriente::detDdiDuSommet(int id) const
{
    return ddi[id-1];
}

void grapheOriente::det_degres()
{
    int n = getNbSommets();

    for (int i = 0; i < n; i++)
    {
        dde.push_back(0);
        ddi.push_back(0);
    }
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            if (matADJ[i][j]!=PAS_DE_LIEN)
            {
                dde[i]++;
                ddi[j]++;
            }
        }
    }
}

void grapheOriente::inverserLien(lien l)
{
    int p=matADJ[l.a.id-1][l.b.id-1];
    matADJ[l.b.id-1][l.a.id-1]=p;
    matADJ[l.a.id-1][l.b.id-1]=PAS_DE_LIEN;
}
