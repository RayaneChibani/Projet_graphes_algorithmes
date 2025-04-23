#ifndef GRAPHEORIENTE_H
#define GRAPHEORIENTE_H

#include "graphe.h"

class grapheOriente : public graphe {
public:
    grapheOriente(std::vector<std::vector<int>> matADJ, std::vector<sommet> sommets);
    grapheOriente(vector<int> fs, vector<int> aps, vector<sommet> sommets);
    grapheOriente(std::vector<int> fs, std::vector<int> aps, std::vector<int> couts, std::vector<sommet> sommets);
    grapheOriente (std::string nomDuFichier);
    grapheOriente(liste_Principale_Secondaire* listePrincipaleSecondaire, std::vector<sommet> sommets);
    void ajouterLien(lien l) override; //
    void supprimerLien(lien l) override; //
    void setPoidsDe(lien l) override; //
    int detDdeDuSommet(int id) const; //
    int detDdiDuSommet(int id) const; //
    void inverserLien(lien l); //
    void ajouterSommet(const sommet& s) override;
private:
    std::vector<int> ddi;
    std::vector<int> dde;
    void det_dde_ddi();
};

#endif
