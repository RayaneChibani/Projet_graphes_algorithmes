#include "grapheNonOriente.h"

grapheNonOriente::grapheNonOriente(std::vector<std::vector<int>> matADJ, std::vector<sommet> sommets): graphe(matADJ, sommets)
{
    det_degres();
    mettreAjourLesIdDesSommets();
}

grapheNonOriente::grapheNonOriente(std::vector<int> fs, std::vector<int> aps, std::vector<int> couts, std::vector<sommet> sommets) : graphe(fs, aps, couts, sommets)
{
    det_degres();
    mettreAjourLesIdDesSommets();
}

grapheNonOriente::grapheNonOriente (std::string nomDuFichier): graphe(nomDuFichier)
{
    det_degres();
    //mettreAjourLesIdDesSommets();
}

grapheNonOriente::grapheNonOriente(liste_Principale_Secondaire* listePrincipaleSecondaire, std::vector<sommet> sommets): graphe(listePrincipaleSecondaire,sommets)
{
    det_degres();
    mettreAjourLesIdDesSommets();
}

void grapheNonOriente::ajouterLien(lien l)
{
    // Si l'arête n'existe pas encore, on l'ajoute
    if (matADJ[l.a.id - 1][l.b.id - 1] == PAS_DE_LIEN) {
        this->m++;
        matADJ[l.a.id - 1][l.b.id - 1] = l.poids;
        matADJ[l.b.id - 1][l.a.id - 1] = l.poids;

        degres[l.a.id - 1]++;
        degres[l.b.id - 1]++;

        qDebug() << "Arête ajoutée entre les sommets" << l.a.nom << "et" << l.b.nom << "poids" << l.poids;
    } else {
        qDebug() << "Arête déjà existante entre les sommets" << l.a.nom << "et" << l.b.nom;
    }
}

void grapheNonOriente::supprimerLien(lien l)
{
    //je supprime un lien que quand le lien existe
    if(matADJ[l.a.id-1][l.b.id-1]!=PAS_DE_LIEN){
        this->m--;
        //le nombre d'arc diminue
        matADJ[l.a.id-1][l.b.id-1]=PAS_DE_LIEN;
        matADJ[l.b.id-1][l.a.id-1]=PAS_DE_LIEN;
        degres[l.a.id-1]--;
        degres[l.b.id-1]--;
    }
}

void grapheNonOriente::setPoidsDe(lien l)
{
    //je modifie le poids d'un lien que quand le lien existe
    if(matADJ[l.a.id-1][l.b.id-1]!=PAS_DE_LIEN){
        matADJ[l.a.id-1][l.b.id-1]=l.poids;
        matADJ[l.b.id-1][l.a.id-1]=l.poids;
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
    for (int i = 0; i < n; i++)
    {
        degres.push_back(0);
    }
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
