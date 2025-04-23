#ifndef GRAPHENONORIENTE_H
#define GRAPHENONORIENTE_H

#include "graphe.h"

class grapheNonOriente : public graphe {
public:
    grapheNonOriente(std::vector<std::vector<int>> matADJ, std::vector<sommet> sommets);
    grapheNonOriente(vector<int> fs, vector<int> aps, vector<sommet> sommets);
    grapheNonOriente(std::vector<int> fs, std::vector<int> aps, std::vector<int> couts, std::vector<sommet> sommets);
    grapheNonOriente (std::string nomDuFichier);
    grapheNonOriente(liste_Principale_Secondaire* listePrincipaleSecondaire, std::vector<sommet> sommets);
    void ajouterLien(lien l) override; //
    void supprimerLien(lien l) override; //
    void setPoidsDe(lien l) override; //
    int detDegreDuSommet(int id) const; //
    void ajouterSommet(const sommet& s) override;
private:
    void det_degres();
    std::vector<int> degres;
};

#endif
