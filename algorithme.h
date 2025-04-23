#ifndef ALGORITHME_H
#define ALGORITHME_H

#include "graphe.h"
#include "grapheoriente.h"
#include "graphenonoriente.h"
#include <QDebug>
#include <QMessageBox>

using std::vector;

class algorithme
{
public:
    algorithme();
    vector<vector<int>> distances(vector<int> aps, vector<int> fs) const;
    bool rang(vector<int> fs, vector<int> aps, vector<int> &rang, vector<int> &prem, vector<int> &pilch);
    void fortconnexe(vector<int>& aps, vector<int>& fs, vector<int>& prem, vector<int>& pilch,
                                 vector<int>& cfc, vector<int>& pred);
    void grapheReduit(vector<int> prem, vector<int> pilch, vector<int> cfc, vector<int> fs, vector<int> aps, vector<int>& fsr, vector<int>& apsr);
    void base_Greduit(vector<int> apsr, vector<int> fsr, vector<int>& br);
    vector<vector<int>> edition_bases(vector<int> prem, vector<int> pilch, vector<int> br);
    void det_points_articulation(grapheNonOriente gr, vector<sommet>& points_articulation);
    void det_isthmes(grapheNonOriente gr, vector<lien>& isthmes);
    void ordonnancement(vector<int> fp, vector<int> app, vector<int> d, vector<int> &fpc, vector<int> &appc, vector<int> &lc);
    void dijkstra(const grapheOriente &gr, int s, vector<int> & d, vector<int> & pr);
    bool dantzig(vector<vector<int>> & matrice);
    void fusion(int s, int t, vector<int>& Nb, vector<int> &prem, vector<int> &pilch, vector<int> &cfc);
    void creerAretes(const graphe& G, vector<lien>& aretes);
    vector<lien> kruskal(vector<lien>& aretes, vector<int>& prem, vector<int> &pilch, vector<int> &cfc);
private:
    void empiler(int x, vector<int>& pilch);
    int depiler(vector<int>& pilch);
    void traversee(int s, vector<int>& aps, vector<int>& fs, vector<int>& num, vector<int>& ro,
                               vector<int>& pred, vector<int>& pilch, vector<int>& cfc, vector<int>& prem,
                               vector<int>& tarj, vector<bool>& entarj, int& p, int& k);
    void explorer(vector<int>& aps, vector<int>& fs, int s, vector<int>& cc, int& c);
    void parcoursProfondeurRec(vector<int>& aps, vector<int>& fs, vector<int>& cc);
    bool lienDejaPresent(const vector<lien>& isthmes, const lien& l);
};

#endif // ALGORITHME_H
