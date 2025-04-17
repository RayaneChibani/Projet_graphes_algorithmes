#ifndef GRAPHE_H
#define GRAPHE_H

#include <iostream>
#include <vector>
#include <memory>
#include <fstream>
#include <sstream>
#include <string>
using namespace std;
struct sommet
{
    int id;
    int recipient5L;
    int recipient3L;
    string toString() const
    {
        return "("+to_string(recipient5L)+","+to_string(recipient3L)+")";
    }
    //methode de lecture d'un sommet a partir d'un fichier
    void lectureDepuisFichier(ifstream& file)
    {
        file >>recipient5L;
        file >>recipient3L;
    }
    //methode d'ecriture d'un sommet dans un fichier
    void ecrireDansFichier(fstream& file) const
    {
        file <<recipient5L<<" ";
        file <<recipient3L;
    }
} ;

struct lien
{
    int a,b,poids;
    string toString() const
    {
        return "("+to_string(a)+","+to_string(b)+")"+"="+to_string(poids);
    }
} ;

struct liste_Principale_Secondaire
{
    int id;
    liste_Principale_Secondaire * suivant; // lien vers le prochain sommet/arrete du graphe
    liste_Principale_Secondaire * lien; //lien vers l'arret sortante/le sommet successeur du graphe
} ;
const int PAS_DE_LIEN = INT_MAX;
class graphe
{
public:
    graphe(vector<vector<int>> matADJ, vector<sommet> sommets);
    graphe(vector<int> fs, vector<int> aps, vector<sommet> sommets);
    graphe(vector<int> fs, vector<int> aps, vector<int> couts, vector<sommet> sommets);
    graphe (string nomDuFichier);
    graphe(liste_Principale_Secondaire* listePrincipaleSecondaire, vector<sommet> sommets);
    liste_Principale_Secondaire* det_liste_secondaire_principale() const;
    void afficherListe(liste_Principale_Secondaire* l) const;
    liste_Principale_Secondaire* from_FS_APS_COUTS2liste_secondaire_principale(vector<int> fs, vector<int> aps, vector<int> couts) const;
    void from_liste_secondaire_principale2FS_APS_COUTS(liste_Principale_Secondaire* l, vector<int> & fs, vector<int>& aps, vector<int>& couts) const;
    //virtual ~graphe() = default;
    vector<vector<int>> getMadj() const; //
    vector<sommet> getSommets() const; //
    int getNbLiens() const; //
    int getNbSommets() const; //
    int getPoidsDe(lien l) const; //
    sommet getSommet(int id) const; //

    virtual void setPoidsDe(lien l)=0;
    void setSommet(int id, sommet s);//

    void ajouterSommet(sommet s); //
    void supprimerSommet(int id); //


    virtual void ajouterLien(lien l)=0;
    virtual void supprimerLien(lien l)=0;

    void det_fs_aps_couts(vector<int>& fs, vector<int>& aps, vector<int>& couts) const;//
    void det_aps_from_fs(vector<int> fs, vector<int>& aps) const;//
    //bool estOriente() const;

    void afficherMatriceAdjacence() const; //
    void afficherSommets();//
    void ecrireLeGrapheDansFichier() const; //
    void inverserDeuxSommets(int idA, int idB);//
    bool aDesPoidsNegatifs() const; //
protected :
//est-ce-que getMatdj retourne une copie de la matrice ou bien la matrice elle meme:
    vector<vector<int>> matADJ;
    int m,n;
    void mettreAjourLesIdDesSommets();
private:
    vector<sommet> sommets;
    int calculNbLiens() const;

};
#endif
