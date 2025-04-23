#include "algorithme.h"

algorithme::algorithme() {}

void algorithme::empiler(int x, vector<int>& pilch)
{
    pilch[x] = pilch[0];
    pilch[0] = x;
}

int algorithme::depiler(vector<int>& tarj)
{
    int s=tarj[0];
    tarj[0]=tarj[tarj[0]];
    return s;
}

//--------------------------------------------------------------------------------DISTANCES
vector<vector<int>> algorithme::distances(vector<int> aps, vector<int> fs) const
{
    int nbSommet = aps[0];
    vector<vector<int>> dist(nbSommet+1, vector<int>(nbSommet+1));

    for(int i = 1 ; i <= nbSommet ; i++)
    {
        dist[i][0] = nbSommet;
        for(int j = 1 ; j <= nbSommet ; j++)
            dist[i][j] = (i == j) ? 0 : -1;
    }

    for(int i = 1 ; i <= nbSommet ; i++)
    {
        vector<int> file;
        vector<bool> visite(nbSommet + 1, false);
        int debut = 0;
        int distance = 1;

        file.push_back(i);
        file.push_back(0);
        visite[i] = true;

        while(debut < (int)file.size())
        {
            if(file[debut] == 0)
            {
                distance++;
                if(debut + 1 < (int)file.size())
                    file.push_back(0);
            }
            else
            {
                int sommetCourant = file[debut];
                int j = aps[sommetCourant];
                int k = 0;
                while(fs[j + k] != 0)
                {
                    int voisin = fs[j + k];
                    if(!visite[voisin])
                    {
                        dist[i][voisin] = distance;
                        file.push_back(voisin);
                        visite[voisin] = true;
                    }
                    k++;
                }
            }
            debut++;
        }
    }
    return dist;
}

//--------------------------------------------------------------------------------RANG
bool algorithme::rang(vector<int> fs, vector<int> aps, vector<int> &rang, vector<int> &prem, vector<int> &pilch) { //Renvoi false si le graphe ne contient aucun sommet ayant un ddi nul au départ.
    int n = aps[0], s, k, h, t;
    rang.resize(n + 1, -1);
    prem.resize(n + 1, 0);
    pilch.resize(n + 1, 0);
    vector<int> ddi(n + 1, 0);

    for (int i = 1; i <= n; i++) {
        for (int j = aps[i]; fs[j] != 0; j++) {
            ddi[fs[j]]++;
        }
    }

    pilch[0] = 0;
    for (s = 1; s <= n; s++) {
        if (ddi[s] == 0) empiler(s, pilch);
    }

    //Si aucun sommet de départ, on retourne false (aucun sommet n'a un ddi nul)
    if (pilch[0] == 0) {
        return false;
    }

    //Calcul des rangs
    k = -1;
    s = pilch[0];
    prem[0] = s;

    while (pilch[0] > 0) {
        k++;
        pilch[0] = 0;
        while (s > 0) {
            rang[s] = k;
            h = aps[s];
            t = fs[h];
            while (t > 0) {
                ddi[t]--;
                if (ddi[t] == 0) empiler(t, pilch);
                h++;
                t = fs[h];
            }
            s = pilch[s];
        }
        s = pilch[0];
        prem[k + 1] = s;
    }

    rang[0] = aps[0];

    return true;
}

//--------------------------------------------------------------------------------TARJAN

void algorithme::traversee(int s, vector<int>& aps, vector<int>& fs, vector<int>& num, vector<int>& ro,
               vector<int>& pred, vector<int>& pilch, vector<int>& cfc, vector<int>& prem,
               vector<int>& tarj, vector<bool>& entarj, int& p, int& k)
{
    int t;
    p++;
    num[s] = p;
    ro[s] = p;
    empiler(s, tarj);
    entarj[s] = true;

    for (int i = aps[s]; (t = fs[i]) != 0; i++)
    {
        if (num[t] == 0)
        {
            pred[t] = s;
            traversee(t, aps, fs, num, ro, pred, pilch, cfc, prem, tarj, entarj, p, k);
            if (ro[t] < ro[s])
            {
                ro[s] = ro[t];
            }
        }
        else if ((num[t] < ro[s]) && entarj[t])
        {
            ro[s] = num[t];
        }
    }

    if (ro[s] == num[s])
    {
        k++;
        int u;
        while (!tarj.empty())
        {
            u = depiler(tarj);
            entarj[u] = false;
            empiler(u, pilch);
            cfc[u] = k;
            if (u == s) break;
        }

        prem[k] = pilch[0];
        pilch[0] = 0;
    }
}

void algorithme::fortconnexe(vector<int>& aps, vector<int>& fs, vector<int>& prem, vector<int>& pilch,
                 vector<int>& cfc, vector<int>& pred)
{
    int n = aps[0];
    prem.resize(n + 1);
    pilch.resize(n + 1);
    cfc.resize(n + 1);
    pred.resize(n + 1);

    vector<int> tarj(n + 1);
    vector<bool> entarj(n + 1);

    vector<int> num(n + 1);
    vector<int> ro(n + 1);

    int k = 0;
    int p = 0;

    // Initialisation des vecteurs
    for (int i = 1; i <= n; i++)
    {
        num[i] = 0;
        pred[i] = 0;
        ro[i] = 0;
        entarj[i] = false;
    }

    pilch[0] = 0;
    tarj[0] = 0;

    for (int s = 1; s <= n; s++)
    {
        if (num[s] == 0)
        {
            traversee(s, aps, fs, num, ro, pred, pilch, cfc, prem, tarj, entarj, p, k);
        }
    }

    prem[0] = k;
    cfc[0] = k;
}

void algorithme::grapheReduit(vector<int> prem, vector<int> pilch, vector<int> cfc, vector<int> fs, vector<int> aps, vector<int>& fsr, vector<int>& apsr)
{
    int s, kr=1, k, t, nbc=prem[0];
    vector<bool> deja_mis;
    deja_mis.resize(nbc+1);
    fsr.resize(fs[0]+1);
    apsr.resize(nbc+1);
    apsr[0]=nbc;
    for(int i=1; i<=nbc; i++)
    {
        apsr[i]=kr;
        for(int j=1; j<=nbc; j++)
        {
            deja_mis[j]=false;
        }
        deja_mis[i]=true;
        s=prem[i];
        while(s!=0)
        {
            for(k=aps[s]; (t=fs[k])!=0; k++)
            {
                if(!deja_mis[cfc[t]])
                {
                    fsr[kr]=cfc[t];
                    kr++;
                    deja_mis[cfc[t]]=true;
                }
            }
            s=pilch[s]; // passer au sommet suivant
        }
        fsr[kr]=0;
        kr++;
    }
    fsr[0]=kr-1;
    deja_mis.erase(deja_mis.begin(), deja_mis.end());
}

void algorithme::base_Greduit(vector<int> apsr, vector<int> fsr, vector<int>& br)
{
    int nr = apsr[0]; // nombre de CFC
    vector<int> ddir(nr + 1, 0);
    br.clear();

    // Calcul des degrés entrants
    for (int c = 1; c <= nr; ++c) {
        for (int k = apsr[c]; fsr[k] != 0; ++k) {
            int succ = fsr[k];
            if (succ >= 1 && succ <= nr) {
                ddir[succ]++;
            }
        }
    }

    // Détermination des bases
    br.push_back(0); // br[0] = nombre de bases (sera rempli à la fin)
    for (int c = 1; c <= nr; ++c) {
        if (ddir[c] == 0) {
            br.push_back(c);
        }
    }

    br[0] = br.size() - 1; // Mise à jour du nombre de bases
}


vector<vector<int>> algorithme::edition_bases(vector<int> prem, vector<int> pilch, vector<int> br)
{
    int nb = br[0]; // Nombre de CFC de l’unique base du graphe réduit

    vector<vector<int>> bases;  // Conteneur pour stocker les bases, chaque base est une liste de sommets
    vector<int> Base;
    Base.resize(nb + 1);  // Pile qui mémorise les sommets des bases du graphe initial
    Base[0] = nb;
    int p = 1;
    int som = prem[br[1]]; // Premier sommet de la première CFC

    while (p >= 1)
    {
        if ((p <= nb) && (som != 0))
        {
            Base[p] = som;  // Ajouter le sommet à la pile de la base
            p++;  // Incrémenter l'indice de la pile
            if (p <= nb)
            {
                som = prem[br[p]];  // Récupérer le sommet suivant à traiter
            }
            else
            {
                // Une fois la base complète, ajouter la base à la liste des bases
                vector<int> baseCourante;
                for (int i = 1; i <= nb; i++)
                {
                    baseCourante.push_back(Base[i]);
                }
                bases.push_back(baseCourante);  // Ajouter la base à la liste des bases

                cout << "{";
                for (int i = 1; i <= nb; i++)
                {
                    cout << Base[i] << " ";
                }
                cout << "}" << endl;
            }
        }
        else
        {
            p--;  // Dépiler un sommet (retourner à la base précédente)
            som = pilch[Base[p]];  // Récupérer le sommet suivant à partir de pilch
        }
    }

    return bases;  // Retourner le conteneur contenant toutes les bases
}

//--------------------------------------------------------------------------------POINTS D'ARTICULATION ET ISTHMES
void algorithme::explorer(vector<int>& aps, vector<int>& fs, int s, vector<int>& cc, int& c)
{
    cc[s]=c;
    int t;
    for (int i = aps[s]; (t = fs[i]) != 0; i++)
    {
        if(cc[t]==0)
            explorer(aps, fs, t ,cc, c);
    }
}

void algorithme::parcoursProfondeurRec(vector<int>& aps, vector<int>& fs, vector<int>& cc)
{

    cc.resize(aps[0] + 1);
    int c=1;
    for(int i=1;i<=aps[0];i++)
        if(cc[i]==0){
            explorer(aps, fs, i ,cc, c);
            c++;
        }
    cc[0]=*max_element(cc.begin(), cc.end());
}

bool algorithme::lienDejaPresent(const vector<lien>& isthmes, const lien& l)
{
    // Vérifie si le lien existe déjà dans les deux sens (l1 - l2 ou l2 - l1)
    for (const lien& existingLien : isthmes) {
        if ((existingLien.a.id == l.a.id && existingLien.b.id == l.b.id) ||
            (existingLien.a.id == l.b.id && existingLien.b.id == l.a.id)) {
            return true;
        }
    }
    return false;
}

void algorithme::det_points_articulation(grapheNonOriente gr, vector<sommet>& points_articulation)
{
    vector<int> cc;
    vector<int> aps;
    vector<int> fs;
    vector<int> couts;
    grapheNonOriente gr2 = gr;
    int t, f;

    gr.det_fs_aps_couts(fs, aps, couts);

    if (aps.empty()) {
        cout << "Aucun sommet dans le graphe" << endl;
        return;
    }

    parcoursProfondeurRec(aps, fs, cc);
    int c = cc[0];
    cout << "Nombre de composantes connexes : " << c << endl;

    bool articulationTrouvee = false;
    for (int i = 1; i <= aps[0]; i++) {
        gr.supprimerSommet(i);
        gr.det_fs_aps_couts(fs, aps, couts);
        cc.clear();
        parcoursProfondeurRec(aps, fs, cc);
        cout << "Sommet " << i << " nb de composantes connexes : " << cc[0] << endl;

        if (cc[0] > c) {
            points_articulation.push_back(gr2.getSommet(i));
            articulationTrouvee = true;
        }
        gr = gr2;
    }

    if (!articulationTrouvee) {
        cout << "Aucun point d'articulation trouvé." << endl;
    }
}

void algorithme::det_isthmes(grapheNonOriente gr, vector<lien>& isthmes)
{
    vector<int> cc;
    vector<int> aps;
    vector<int> fs;
    vector<int> couts;
    grapheNonOriente gr2 = gr;
    int t, f;

    gr.det_fs_aps_couts(fs, aps, couts);

    if (aps.empty()) {
        cout << "Aucun sommet dans le graphe" << endl;
        return;
    }

    parcoursProfondeurRec(aps, fs, cc);
    int c = cc[0];
    cout << "Nombre de composantes connexes : " << c << endl;

    bool isthmeTrouve = false;
    for (int i = 1; i <= aps[0]; i++) {
        t = aps[i];
        while (fs[t] != 0) {
            f = fs[t];
            lien l1 = {gr.getSommet(i), gr.getSommet(f)};

            gr.supprimerLien(l1);

            gr.det_fs_aps_couts(fs, aps, couts);
            cc.clear();
            parcoursProfondeurRec(aps, fs, cc);
            cout << "Lien " << i << "," << f << " nb de composantes connexes : " << cc[0] << endl;
            if (cc[0] > c) {
                sommet s1 = gr.getSommet(i);
                sommet s2 = gr.getSommet(f);

                lien l2 = {s1, s2};

                if (!lienDejaPresent(isthmes, l2)) {
                    isthmes.push_back(l2);
                    isthmeTrouve = true;
                }
            }

            t++;
            gr = gr2;
        }
    }

    if (!isthmeTrouve) {
        cout << "Aucun isthme trouvé." << endl;
    }
}

//--------------------------------------------------------------------------------ORDONNANCEMENT
void algorithme::ordonnancement(vector<int> fp, vector<int> app, vector<int> d, vector<int> &fpc, vector<int> &appc, vector<int> &lc) {
    int n = app[0], m = fp[0];
    fpc.resize(m + 1);
    appc.resize(n + 1);
    appc[0] = n;
    lc.resize(n + 1);
    lc[0] = n;

    int kc, t, lg;
    lc[1] = 0;
    fpc[1] = 0; // fin de la liste
    appc[1] = 1;

    kc = 1; // indice de la dernière place remplie dans fpc
    for (int s = 2; s <= n; s++) {
        lc[s] = 0;
        appc[s] = kc + 1;
        for (int k = app[s]; (t = fp[k]) != 0; k++) {
            lg = lc[t] + d[t];
            if (lg >= lc[s]) {
                if (lg > lc[s]) {
                    lc[s] = lg;
                    kc = appc[s];
                    fpc[kc] = t;
                } else {
                    kc++;
                    fpc[kc] = t;
                }
            }
        }
        kc++;
        fpc[kc] = 0;
    }
    fpc[0] = kc;
}

//--------------------------------------------------------------------------------DIJKSTRA
void algorithme::dijkstra(const grapheOriente &gr, int s, vector<int> & d, vector<int> & pr)
{
    if (gr.aDesPoidsNegatifs())
    {
        qDebug() << "Impossible d'implementer l'algorithme de djikstra, le graphe possède des poids negatifs\n";
        return;
    }
    vector<int> fs;
    vector<int> aps;
    vector<int> couts;
    vector<vector<int>> matrice = gr.getMadj();
    gr.det_fs_aps_couts(fs, aps,couts);
    int ind; /* nombre d'élements qui restent à traiter */
    int i, j, k, v, m;
    int n = aps[0];
    vector<bool> x_bar;
    //sert à dire quels sont les sommets qui restent à traiter
    //x_bar[i] = false ne fait plus parti des sommets à visiter
    //x_bar[i] = true fait parti des sommets à visiter

    //initialisation des tableaux d, pr et inS
    for (int i = 0; i < n; i++)
    {
        x_bar.push_back(true);
        d.push_back(matrice[s-1][i]);
        if(d[i]!=INT_MAX)
            pr.push_back(1);
        else
            pr.push_back(INT_MAX);
    }

    d[s-1] = 0;
    pr[s-1] = 0;
    x_bar[s-1] = false; /* on supprime le sommet 1 */
    ind = n-1; //ind revient au nombre de sommets du graphe qui restent à traiter
    while (ind >0)
    {
        /* calcul du minimum selon d des sommets de S */
        m = INT_MAX;
        for (i=0; i<n; i++)
            if (x_bar[i] == true)
                if (d[i] < m)
                {
                    m = d[i];
                    j = i;
                }
        if (m == INT_MAX) return;
        x_bar[j] = false;
        ind--;
        k = aps[j+1];
        while(fs[k] != 0)
        {
            if (x_bar[fs[k]-1] == true)
            {
                v = d[j]+matrice[j][fs[k]-1];
                if (v < d[fs[k]-1])
                {
                    d[fs[k]-1] = v;
                    pr[fs[k]-1] = j+1;
                }
            }
            k++;
        }
    }
}

//--------------------------------------------------------------------------------DANTZIG
bool algorithme::dantzig(vector<vector<int>> & matrice)
{
    double x;
    int n=matrice.size();
    for(int k=1; k<n-1; k++)
    {
        for(int i=0; i<=k; i++)
        {
            for(int j=0; j<=k; j++)
            {

                if(matrice[i][j]==PAS_DE_LIEN || matrice[j][k+1]==PAS_DE_LIEN)
                    x=PAS_DE_LIEN;
                else
                    x=matrice[i][j]+matrice[j][k+1];
                if(x<matrice[i][k+1])
                    matrice[i][k+1]=x;

                if(matrice[j][i]==PAS_DE_LIEN || matrice[k+1][j]==PAS_DE_LIEN)
                    x=PAS_DE_LIEN;
                else
                    x=matrice[j][i]+matrice[k+1][j];
                if(x<matrice[k+1][i])
                    matrice[k+1][i]=x;

            }

            if(matrice[k+1][i]!=PAS_DE_LIEN && matrice[i][k+1]!=PAS_DE_LIEN && (matrice[k+1][i]+matrice[i][k+1])<0)
            {
                qDebug() << "Presence d'un circuit absorbant\n";
                qDebug() << "Provient par " << i+1 << " et " << k+2 << '\n';

                return false;
            }
        }
        for(int i=0; i<=k; i++)
            for(int j=0; j<=k; j++)
            {
                if((x=matrice[i][k+1]+matrice[k+1][j])<matrice[i][j])
                    matrice[i][j]=x;
            }
    }
    return true;
}

//--------------------------------------------------------------------------------KRUSKAL

void algorithme::fusion(int s, int t, vector<int>& Nb, vector<int>& prem, vector<int>& pilch, vector<int>& cfc)
{
    int cs = cfc[s];
    int ct = cfc[t];

    if (Nb[cs] < Nb[ct]) {
        std::swap(cs, ct);
    }

    int x = prem[ct];
    cfc[x] = cs;

    while (pilch[x] != 0) {
        x = pilch[x];
        cfc[x] = cs;
    }

    pilch[x] = prem[cs];
    prem[cs] = prem[ct];
    Nb[cs] += Nb[ct];
}

void algorithme::creerAretes(const graphe& G, vector<lien>& aretes)
{
    int n = G.getNbSommets(); // Nombre de sommets

    // Parcourir la matrice d'adjacence
    for (int i = 0; i < n; i++) {
        for (int j = i + 1; j < n; j++) { // j = i+1 pour éviter les doublons
            if (G.getMadj()[i][j] != INT_MAX) { // S'il y a une arête entre i et j
                // Créer des objets sommet en accédant directement aux sommets du graphe
                sommet depart = G.getSommets()[i];  // Récupérer le sommet 'i'
                sommet arrivee = G.getSommets()[j]; // Récupérer le sommet 'j'

                // Ajouter l'arête avec les sommets et le poids
                aretes.push_back({depart, arrivee, G.getMadj()[i][j]});
            }
        }
    }

    // Affichage des arêtes
    qDebug() << "Arêtes du graphe :\n";
    for (const auto& e : aretes) {
        qDebug() << "(" << e.a.id << ", " << e.b.id << ") poids: " << e.poids << "\n";
    }
}

vector<lien> algorithme::kruskal(vector<lien>& aretes, vector<int>& prem, vector<int>& pilch, vector<int>& cfc)
{
    map<int, map<int, vector<lien>>> aretesParPoidsEtSommet;

    set<int> tousLesSommets;

    int maxId = 0;
    for (const auto& e : aretes) {
        aretesParPoidsEtSommet[e.poids][e.a.id].push_back(e);
        aretesParPoidsEtSommet[e.poids][e.b.id].push_back({e.b, e.a, e.poids});

        tousLesSommets.insert(e.a.id);
        tousLesSommets.insert(e.b.id);

        maxId = max({maxId, e.a.id, e.b.id});
    }

    int n = tousLesSommets.size();

    int taille = maxId + 1;

    vector<int> Nb(taille, 0);
    prem.resize(taille);
    pilch.resize(taille);
    cfc.resize(taille);

    for (int i = 1; i < taille; ++i) {
        prem[i] = i;
        pilch[i] = 0;
        cfc[i] = i;
        Nb[i] = 1;
    }

    int nbAretesAjoutees = 0;
    vector<lien> liensArbreCouvrant;

    //Parcours par poids croissants
    for (const auto& [poids, mapSommets] : aretesParPoidsEtSommet) {
        //Parcours des sommets pour ce poids
        for (const auto& [sommetId, liensDuSommet] : mapSommets) {
            //Parcours des arêtes de ce sommet
            for (const auto& lien : liensDuSommet) {
                int u = lien.a.id;
                int v = lien.b.id;

                if (cfc[u] != cfc[v]) {
                    liensArbreCouvrant.push_back(lien);
                    fusion(u, v, Nb, prem, pilch, cfc);

                    qDebug() << "Ajout de l'arete" << lien.a.nom << " , " << lien.b.nom << "(valeur = " << lien.poids << ") à l'arbre T.";

                    nbAretesAjoutees++;
                    if (nbAretesAjoutees == n - 1)
                        return liensArbreCouvrant; //On s'arrete à n-1 arêtes
                }
            }
        }
    }

    return liensArbreCouvrant;
}
