#include "grapheNonOriente.h"

grapheNonOriente::grapheNonOriente(std::vector<std::vector<int>> matADJ, std::vector<sommet> sommets): graphe(matADJ, sommets)
{
    det_degres();
    mettreAjourLesIdDesSommets();
}

grapheNonOriente::grapheNonOriente(vector<int> fs, vector<int> aps, vector<sommet> sommets) : grapheNonOriente(fs, aps, vector<int>(fs[0] + 1, 1), sommets) {}

grapheNonOriente::grapheNonOriente(std::vector<int> fs, std::vector<int> aps, std::vector<int> couts, std::vector<sommet> sommets) : graphe(fs, aps, couts, sommets)
{
    det_degres();
    mettreAjourLesIdDesSommets();
}

grapheNonOriente::grapheNonOriente (std::string nomDuFichier): graphe(nomDuFichier)
{
    det_degres();
    mettreAjourLesIdDesSommets();
}

grapheNonOriente::grapheNonOriente(liste_Principale_Secondaire* listePrincipaleSecondaire, std::vector<sommet> sommets): graphe(listePrincipaleSecondaire,sommets)
{
    det_degres();
    mettreAjourLesIdDesSommets();
}

void grapheNonOriente::ajouterLien(lien l)
{
    //j'ajoute un lien que quand ya pas de lien existant
    if(matADJ[l.a-1][l.b-1]==PAS_DE_LIEN)
    {
        this->m++;
        //le nombre d'arc augmente
        matADJ[l.a-1][l.b-1]=l.poids;
        matADJ[l.b-1][l.a-1]=l.poids;

        degres[l.a-1]++;
        degres[l.b-1]++;
    }
}

void grapheNonOriente::supprimerLien(lien l)
{
    //je supprime un lien que quand le lien existe
    if(matADJ[l.a-1][l.b-1]!=PAS_DE_LIEN){
        this->m--;
        //le nombre d'arc diminue
        matADJ[l.a-1][l.b-1]=PAS_DE_LIEN;
        matADJ[l.b-1][l.a-1]=PAS_DE_LIEN;
        degres[l.a-1]--;
        degres[l.b-1]--;
    }
}

void grapheNonOriente::setPoidsDe(lien l)
{
    //je modifie le poids d'un lien que quand le lien existe
    if(matADJ[l.a-1][l.b-1]!=PAS_DE_LIEN){
        matADJ[l.a-1][l.b-1]=l.poids;
        matADJ[l.b-1][l.a-1]=l.poids;
        //le nombre d'arc reste le meme
    }
}

int grapheNonOriente::detDegreDuSommet(int id) const
{
    return degres[id-1];
}

void grapheNonOriente::det_degres()
{
    int n = getNbSommets();
    degres.resize(n);
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            if (matADJ[i][j]!=PAS_DE_LIEN)
            {
                degres[i]++;
                degres[j]++;
            }
        }
    }
}
