<<<<<<< HEAD
#include "graphe.h"

graphe::graphe(vector<vector<int>> matADJ, vector<sommet> sommets): matADJ(move(matADJ)), sommets(move(sommets))
{
    mettreAjourLesIdDesSommets();
    this->n = this->sommets.size();
    this->m=calculNbLiens();
}

graphe::graphe(vector<int> fs, vector<int> aps, vector<int> couts, vector<sommet> sommets): matADJ(aps[0], vector<int>(aps[0], PAS_DE_LIEN)), sommets(move(sommets))
{
    int n = aps[0];  // Nombre de sommets
    // Remplissage de la matrice d'adjacence à partir de FS et APS
    int m = fs[0] - aps[0]; // Nombre d'arcs
    this->m=m;
    this->n=n;
    mettreAjourLesIdDesSommets();
    int i = 0, j = 1;
    while (i < n)   // Parcourir les sommets
    {
        while (fs[j] != 0)   // Parcourir les successeurs
        {
            this->matADJ[i][fs[j]-1] = couts[j]; // Ajouter l'arête (i, fs[j]-1) avec le cout couts[j]
            j++;
        }
        j++;  // Passer au sommet suivant dans fs
        i++;
    }
}
graphe::graphe(vector<int> fs, vector<int> aps, vector<sommet> sommets) : graphe(fs, aps, vector<int>(fs[0] + 1, 1), sommets) {}
graphe::graphe(string nomDuFichier)
{
//ne pas oublier aussi d'initiliaser le tableau sommets
// suppossons que dans le fichier ya le tableau sommets puis la matrice d'adjacence
    string caractere;
    ifstream fichier(nomDuFichier, ios::in);
    if(!fichier)
    {
        cout << "Impossible d'ouvrir le fichier" << endl;
        return;
    }
    int n, m;
    fichier >> n;
    fichier >> m;
    matADJ.resize(n, vector<int>(n, PAS_DE_LIEN));
    this->n=n;
    this->m=m;
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            fichier >> matADJ[i][j];
        }
    }
    sommets.resize(n);
    //lecture de chaque sommet, a rendre plus generique
    for (int i = 0; i < n; i++)
    {
        sommet s;
        s.id=i+1;
        s.lectureDepuisFichier(fichier);
        sommets[i]=s;
    }
    fichier.close();
}

graphe::graphe(liste_Principale_Secondaire* listePrincipaleSecondaire, vector<sommet> sommets):sommets(move(sommets))
{
    int n=this->sommets.size();
    mettreAjourLesIdDesSommets();
    liste_Principale_Secondaire* r=listePrincipaleSecondaire,*s,*b;
    matADJ.resize(n, vector<int>(n, PAS_DE_LIEN));
    int s1,s2,poids;
    while(r!=nullptr)
    {
        s1=(*r).id;
        b=r->lien; //-> quand b c'est un pointeur et non pas un objet

        while(b!=nullptr)
        {
            s=b->lien;
            s2=(*s).id;
            poids=(*b).id;
            matADJ[s1][s2]=poids;
            b=b->suivant;
        }
        r=r->suivant;
    }
    this->n=n;
    this->m=calculNbLiens();
}
void graphe::mettreAjourLesIdDesSommets()
{
    for (int j =0; j < getNbSommets(); j++)
    {
        sommets[j].id=j+1;
    }
}

liste_Principale_Secondaire* graphe::det_liste_secondaire_principale() const
{
    liste_Principale_Secondaire* k=nullptr;
    liste_Principale_Secondaire* s1;
    vector<liste_Principale_Secondaire*> tabSommets;
    for (int j =0; j < getNbSommets(); j++)
    {
        k= new liste_Principale_Secondaire{j+1, nullptr, nullptr};
        tabSommets.push_back(k);
        if (j > 0)
            tabSommets[j - 1]->suivant = k;  // Lier au précédent

    }
    s1=tabSommets[0];
    for (int j = 0; j < getNbSommets(); j++)
    {
        liste_Principale_Secondaire* premierArc = nullptr;
        liste_Principale_Secondaire* dernierArc = nullptr;

        for (int i = 0; i < getNbSommets(); i++)
        {
            if (matADJ[j][i] != PAS_DE_LIEN)
            {
                liste_Principale_Secondaire* arrete = new liste_Principale_Secondaire{matADJ[j][i], nullptr, tabSommets[i]};

                if (premierArc == nullptr)
                    premierArc = arrete;  // Premier arc du sommet
                else
                    dernierArc->suivant = arrete;  // Lier au précédent

                dernierArc = arrete;
            }
        }
        tabSommets[j]->lien = premierArc;  // Associer la liste des arcs au sommet
    }

    return s1;
}

void graphe::afficherListe(liste_Principale_Secondaire* l) const
{
    liste_Principale_Secondaire* r=l, * b, *s;
    while(r!=nullptr)
    {
        b=r->lien; //-> quand b c'est un pointeur et non pas un objet
        while(b!=nullptr)
        {
            s=b->lien;
            cout<<(*r).id<<" "<<(*s).id<<endl;
            b=b->suivant;
        }
        r=r->suivant;
    }
}

liste_Principale_Secondaire* graphe::from_FS_APS_COUTS2liste_secondaire_principale(vector<int> fs, vector<int> aps, vector<int> couts) const
{
    liste_Principale_Secondaire* k=nullptr;
    liste_Principale_Secondaire* s1;
    vector<liste_Principale_Secondaire*> tabSommets;
    for (int j =0; j < aps[0]; j++)
    {
        k= new liste_Principale_Secondaire{j+1, nullptr, nullptr};
        tabSommets.push_back(k);
        if (j > 0)
            tabSommets[j - 1]->suivant = k;  // Lier au précédent
    }
    s1=tabSommets[0];

    int i = 1, j = 1;
    while (i <= aps[0])   // Parcourir les sommets
    {
        liste_Principale_Secondaire* premierArc = nullptr;
        liste_Principale_Secondaire* dernierArc = nullptr;
        while (fs[j] != 0)   // Parcourir les successeurs
        {
            liste_Principale_Secondaire* arrete = new liste_Principale_Secondaire{couts[j], nullptr, tabSommets[fs[j]-1]};

            if (premierArc == nullptr)
                premierArc = arrete;  // Premier arc du sommet
            else
                dernierArc->suivant = arrete;  // Lier au précédent

            dernierArc = arrete;
            j++;
        }
        tabSommets[i-1]->lien = premierArc;  // Associer la liste des arcs au sommet
        j++;  // Passer au sommet suivant dans fs
        i++;

    }

    return s1;
}

void graphe::from_liste_secondaire_principale2FS_APS_COUTS(liste_Principale_Secondaire* l, vector<int> & fs, vector<int>& aps, vector<int>& couts) const
{
    liste_Principale_Secondaire* r=l, * b, *s;
    int n=0,m=0;
    while(r!=nullptr)
    {
        n++;
        b=r->lien; //-> quand b c'est un pointeur et non pas un objet
        while(b!=nullptr)
        {
            m++;
            b=b->suivant;
        }
        r=r->suivant;
    }

    aps.push_back(n);
    fs.push_back(n+m);
    couts.push_back(n+m);
    for (int i = 1; i <= n; i++)
    {
        aps.push_back(0);
    }
    for (int i = 1; i <= m+n; i++)
    {
        fs.push_back(0);
        couts.push_back(0);
    }
    int i=1,j=1;
    r=l;
    /*
             arrete12 = new liste_Principale_Secondaire{12, @suivant prochainArc, @lien sommetDestination};
             sommet2 = new liste_Principale_Secondaire{12, prochainSommet, premiereArrete};
             */
    while(r!=nullptr)
    {
        aps[i]=j;
        b=r->lien; //-> quand b c'est un pointeur et non pas un objet
        while(b!=nullptr)
        {
            s=b->lien;
            fs[j]=(*s).id;
            couts[j]=(*b).id;
            b=b->suivant;
            j++;
        }
        fs[j]=0;
        j++;
        i++;
        r=r->suivant;
    }
}

void graphe::det_fs_aps_couts(vector<int>& fs, vector<int>& aps, vector<int>& couts) const
{
    int fs_indice=1;
    int aps_indice=1;
    fs.resize(n + m + 1);
    couts.resize(n + m + 1);
    aps.resize(n + 1);
    //fs.resize(n + m + 1);
    //aps.resize(n + 1);
    fs[0]=n+m;
    couts[0]=n+m;
    aps[0]=n;
    for (int j =0; j < n; j++)
    {
        aps[aps_indice] = fs_indice;
        for (int i = 0; i < n; i++)
        {
            if (this->matADJ[j][i]!=PAS_DE_LIEN)
            {
                fs[fs_indice]=i+1;
                couts[fs_indice]=matADJ[j][i];
                fs_indice++;
            }
        }
        fs[fs_indice]=0;
        fs_indice++;
        aps_indice++;
    }
}

void graphe::det_aps_from_fs(vector<int> fs, vector<int>& aps) const
{

    int nb_sommets=0;
    for (int i=1; i<=fs[0]; i++)
        if(fs[i]==0)
            nb_sommets++;
    aps.resize(nb_sommets+1);
    aps[0]=nb_sommets;
    int j=1;
    aps[j]=1;
    for (int i=1; i<fs[0]; i++)
        if(fs[i]==0)
        {
            j++;
            aps[j]=i+1;
        }

}

void graphe::afficherMatriceAdjacence() const
{
    // Afficher la première ligne (seulement 2 éléments)
    cout << getNbSommets()<< " " << m << "\n";

    // Afficher le reste de la matrice (de 1 à n)
    for (int i = 0; i <getNbSommets(); i++)   // De 1 à n (nb de sommets)
    {
        for (int j = 0; j <getNbSommets(); j++)   // n colonnes
        {
            cout << matADJ[i][j] << " ";
        }
        cout << "\n";
    }
}

/*
bool graphe::estOriente() const
{
    int *ddi ;
    int *dde ;
    int *fs, *aps;
    det_fs_aps(fs, aps);
    det_ddi(aps,fs,ddi);
    det_dde(aps,fs,dde);
    for(int i = 0 ; i<= aps[0]; i++)
    {
        if(ddi[i]!=dde[i])
        {
            return true;
        }

    }
    return false;
}
*/

vector<vector<int>> graphe::getMadj() const
{
    return matADJ;
}
vector<sommet> graphe::getSommets() const
{
    return sommets;
}

int graphe::getNbSommets() const
{
    return n;
}
int graphe::getNbLiens() const
{
    return m;
}

int graphe::calculNbLiens() const
{
    int m=0;

    for (int i = 0; i <getNbSommets(); i++)   // De 1 à n (nb de sommets)
    {
        for (int j = 0; j <getNbSommets(); j++)   // n colonnes
        {
            if(matADJ[i][j]!=PAS_DE_LIEN)
                m++;
        }
    }
    return m;
}

void graphe::afficherSommets()
{
    for (int i = 0; i < getNbSommets(); ++i)
    {
        cout << sommets[i].toString()<< " ";
    }
}

void graphe::setSommet(int id, sommet s)
{
    s.id=sommets[id-1].id;
    sommets[id-1]=s;
}

int graphe::getPoidsDe(lien l) const
{
    return matADJ[l.a-1][l.b-1];
}
/*
void graphe::setPoidsDe(lien l)
{
    matADJ[l.a][l.b]=l.poids;
}
*/
sommet graphe::getSommet(int id) const
{
    return sommets[id-1];
}

void graphe::ajouterSommet(sommet s)
{
    this->n++;
    sommets.push_back(s);
    //matADJ.resize(getNbSommets(), std::vector<int>(getNbSommets(), PAS_DE_LIEN));
    //sommets.resize(getNbSommets());
    //matADJ.resize(nouvelle taille (lignes++ && colonnes++), creation d'une nouvelle ligne mise a PAS_DE_LIEN)
    matADJ.resize(getNbSommets(), vector<int>(getNbSommets(), PAS_DE_LIEN));
    //resize() creera la nouvelle ligne a PAS_DE_LIEN et
    //une nouvelle colonne qui prend ce qui a dans la derniere colonne du tableau avant le resize

    for (int i = 0; i <getNbSommets(); ++i)
    {
        matADJ[i][getNbSommets()-1]=PAS_DE_LIEN;  // Ajouter une nouvelle colonne à chaque ligne
    }
//mettre a jour son id
    sommets[getNbSommets()-1].id=getNbSommets();
}

void graphe::supprimerSommet(int id)
{
    int n=getNbSommets();
    this->n--;
    for (int i = id-1; i < n-1; ++i)
    {
        sommets[i]=sommets[i+1];
        sommets[i].id=i+1;
    }
    sommets.pop_back();

    for (int i = id-1 ; i < n-1; ++i)
    {
        matADJ[i]=matADJ[i+1];
    }
    matADJ.pop_back();
    //suppression de la derniere ligne

    for (int i = 0; i < n-1; ++i)
    {
        for (int j = id-1; j < n-1; ++j)
        {
            matADJ[i][j]=matADJ[i][j+1];
        }
    }

    n=getNbSommets();

    for (int i = 0; i < n; ++i)
    {
        matADJ[i].pop_back();  // Supprimer la derniere colonne à chaque ligne
    }
    this->m=calculNbLiens();
}

bool graphe::aDesPoidsNegatifs() const
{
    for (int i=0; i< getNbSommets(); i++)
    {

        for (int j=0; j< getNbSommets(); j++)
        {
            if ( matADJ[i][j]!=PAS_DE_LIEN && matADJ[i][j]<0 ) return true;
        }

    }
    return false;
}

void graphe::ecrireLeGrapheDansFichier() const
{
    fstream file;
    file.open ("Graphe.txt",fstream::out);
    file << getNbSommets()<<" "<< getNbLiens()<< endl;

    for (int i=0; i< getNbSommets(); i++)   // De 1 à n (nb de sommets)
    {
        for (int j = 0; j <getNbSommets(); j++)   // n colonnes
        {
            file << matADJ[i][j] <<" ";
        }
        file <<endl;
    }
    //ecriture de chaque sommet, a rendre plus generique
    for (int i=0; i< getNbSommets(); i++)   // De 1 à n (nb de sommets)
    {
        sommets[i].ecrireDansFichier(file);
        file <<endl;
    }
    file.close();
}

void graphe::inverserDeuxSommets(int idA, int idB)
{
    //changer les lignes dans la matrice d'adjacence
    //changer les id
    int i=sommets[idA-1].id;
    sommets[idA-1].id=sommets[idB-1].id;
    sommets[idB-1].id=i;
    //changer les structures
    sommet s=sommets[idA-1];
    sommets[idA-1]=sommets[idB-1];
    sommets[idB-1]=s;

    //changer les lignes dans la matrice d'adjacence
    vector<int> ligneA=matADJ[idA-1];
    matADJ[idA-1]=matADJ[idB-1];
    matADJ[idB-1]=ligneA;
}
=======
#include "graphe.h"

graphe::graphe(vector<vector<int>> matADJ, vector<sommet> sommets): matADJ(move(matADJ)), sommets(move(sommets))
{
    //mettreAjourLesIdDesSommets();
    this->n = this->sommets.size();
    this->m=calculNbLiens();
}

graphe::graphe(vector<int> fs, vector<int> aps, vector<int> couts, vector<sommet> sommets) : matADJ(aps[0], vector<int>(aps[0], PAS_DE_LIEN)), sommets(move(sommets))
{
    int n = aps[0];
    int m = fs[0] - aps[0];
    this->m = m;
    this->n = n;
    mettreAjourLesIdDesSommets();

    int i = 0, j = 1;
    while (i < n) {
        while (fs[j] != 0) {
            //Si couts est vide ou trop petit, on met 1 comme poids par défaut
            int cout = (j < (int)couts.size()) ? couts[j] : 1;
            this->matADJ[i][fs[j] - 1] = cout;
            j++;
        }
        j++;
        i++;
    }
}

graphe::graphe(string nomDuFichier)
{
    //ne pas oublier aussi d'initiliaser le tableau sommets
    // suppossons que dans le fichier ya le tableau sommets puis la matrice d'adjacence
    string caractere;
    ifstream fichier(nomDuFichier, ios::in);
    if(!fichier)
    {
        qDebug() << "Impossible d'ouvrir le fichier f\n";
        return;
    }
    int n, m;
    fichier >> n;
    fichier >> m;
    matADJ.resize(n, vector<int>(n, PAS_DE_LIEN));
    this->n=n;
    this->m=m;
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            fichier >> matADJ[i][j];
        }
    }
    sommets.resize(n);
    //lecture de chaque sommet, a rendre plus generique
    for (int i = 0; i < n; i++)
    {
        sommet s;
        s.id=i+1;
        s.lectureDepuisFichier(fichier);
        sommets[i]=s;
    }
    fichier.close();
}

graphe::graphe(liste_Principale_Secondaire* listePrincipaleSecondaire, vector<sommet> sommets):sommets(move(sommets))
{
    int n=this->sommets.size();
    mettreAjourLesIdDesSommets();
    liste_Principale_Secondaire* r=listePrincipaleSecondaire,*s,*b;
    matADJ.resize(n, vector<int>(n, PAS_DE_LIEN));
    int s1,s2,poids;
    while(r!=nullptr)
    {
        s1=(*r).id;
        b=r->lien; //-> quand b c'est un pointeur et non pas un objet

        while(b!=nullptr)
        {
            s=b->lien;
            s2=(*s).id;
            poids=(*b).id;
            matADJ[s1][s2]=poids;
            b=b->suivant;
        }
        r=r->suivant;
    }
    this->n=n;
    this->m=calculNbLiens();
}

void graphe::det_app(vector<int> aps, vector<int> ddi, vector<int> &app)
{
    int n=aps[0];
    app.resize(n + 1);
    app[0] = n;
    app[1] = 1;
    for(int i=1; i<n; i++)
        app[i+1] = app[i] + ddi[i] + 1;
}

void graphe::det_fp(vector<int> fs, vector<int> aps, vector<int> app, vector<int> &fp)
{
    int n = aps[0];
    int m = fs[0];
    fp.resize(m + 1);
    fp[0] = m;
    int k=1,j;
    for(int i=1; i<=n; i++)
    {
        while ((j=fs[k]) > 0)
        {
            fp[app[j]] = i;
            app[j]++;
            k++;
        }
        k++;
    }
    fp[app[n]] = 0;
    for(int i=n-1; i>=1; i--)
    {
        fp[app[i]] = 0;
        app[i+1] = app[i] + 1;
    }
    app[1] = 1;
}

void graphe::mettreAjourLesIdDesSommets()
{
    for (int j =0; j < getNbSommets(); j++)
    {
        sommets[j].id=j+1;
    }
}

liste_Principale_Secondaire* graphe::det_liste_secondaire_principale() const
{
    liste_Principale_Secondaire* k=nullptr;
    liste_Principale_Secondaire* s1;
    vector<liste_Principale_Secondaire*> tabSommets;
    for (int j =0; j < getNbSommets(); j++)
    {
        k= new liste_Principale_Secondaire{j+1, nullptr, nullptr};
        tabSommets.push_back(k);
        if (j > 0)
            tabSommets[j - 1]->suivant = k;  // Lier au précédent

    }
    s1=tabSommets[0];
    for (int j = 0; j < getNbSommets(); j++)
    {
        liste_Principale_Secondaire* premierArc = nullptr;
        liste_Principale_Secondaire* dernierArc = nullptr;

        for (int i = 0; i < getNbSommets(); i++)
        {
            if (matADJ[j][i] != PAS_DE_LIEN)
            {
                liste_Principale_Secondaire* arrete = new liste_Principale_Secondaire{matADJ[j][i], nullptr, tabSommets[i]};

                if (premierArc == nullptr)
                    premierArc = arrete;  // Premier arc du sommet
                else
                    dernierArc->suivant = arrete;  // Lier au précédent

                dernierArc = arrete;
            }
        }
        tabSommets[j]->lien = premierArc;  // Associer la liste des arcs au sommet
    }

    return s1;
}

void graphe::afficherListe(liste_Principale_Secondaire* l) const
{
    liste_Principale_Secondaire* r=l, * b, *s;
    while(r!=nullptr)
    {
        b=r->lien; //-> quand b c'est un pointeur et non pas un objet
        while(b!=nullptr)
        {
            s=b->lien;
            cout<<(*r).id<<" "<<(*s).id<<endl;
            b=b->suivant;
        }
        r=r->suivant;
    }
}

liste_Principale_Secondaire* graphe::from_FS_APS_COUTS2liste_secondaire_principale(vector<int> fs, vector<int> aps, vector<int> couts) const
{
    liste_Principale_Secondaire* k=nullptr;
    liste_Principale_Secondaire* s1;
    vector<liste_Principale_Secondaire*> tabSommets;
    for (int j =0; j < aps[0]; j++)
    {
        k= new liste_Principale_Secondaire{j+1, nullptr, nullptr};
        tabSommets.push_back(k);
        if (j > 0)
            tabSommets[j - 1]->suivant = k;  // Lier au précédent
    }
    s1=tabSommets[0];

    int i = 1, j = 1;
    while (i <= aps[0])   // Parcourir les sommets
    {
        liste_Principale_Secondaire* premierArc = nullptr;
        liste_Principale_Secondaire* dernierArc = nullptr;
        while (fs[j] != 0)   // Parcourir les successeurs
        {
            liste_Principale_Secondaire* arrete = new liste_Principale_Secondaire{couts[j], nullptr, tabSommets[fs[j]-1]};

            if (premierArc == nullptr)
                premierArc = arrete;  // Premier arc du sommet
            else
                dernierArc->suivant = arrete;  // Lier au précédent

            dernierArc = arrete;
            j++;
        }
        tabSommets[i-1]->lien = premierArc;  // Associer la liste des arcs au sommet
        j++;  // Passer au sommet suivant dans fs
        i++;

    }

    return s1;
}

void graphe::from_liste_secondaire_principale2FS_APS_COUTS(liste_Principale_Secondaire* l, vector<int> & fs, vector<int>& aps, vector<int>& couts) const
{
    liste_Principale_Secondaire* r=l, * b, *s;
    int n=0,m=0;
    while(r!=nullptr)
    {
        n++;
        b=r->lien; //-> quand b c'est un pointeur et non pas un objet
        while(b!=nullptr)
        {
            m++;
            b=b->suivant;
        }
        r=r->suivant;
    }

    aps.push_back(n);
    fs.push_back(n+m);
    couts.push_back(n+m);
    for (int i = 1; i <= n; i++)
    {
        aps.push_back(0);
    }
    for (int i = 1; i <= m+n; i++)
    {
        fs.push_back(0);
        couts.push_back(0);
    }
    int i=1,j=1;
    r=l;
    /*
             arrete12 = new liste_Principale_Secondaire{12, @suivant prochainArc, @lien sommetDestination};
             sommet2 = new liste_Principale_Secondaire{12, prochainSommet, premiereArrete};
             */
    while(r!=nullptr)
    {
        aps[i]=j;
        b=r->lien; //-> quand b c'est un pointeur et non pas un objet
        while(b!=nullptr)
        {
            s=b->lien;
            fs[j]=(*s).id;
            couts[j]=(*b).id;
            b=b->suivant;
            j++;
        }
        fs[j]=0;
        j++;
        i++;
        r=r->suivant;
    }
}


void graphe::det_fs_aps_couts(vector<int>& fs, vector<int>& aps, vector<int>& couts) const
{
    int m_total = 0;         // Nombre total de liens (pour dimensionnement)

    fs.clear();
    aps.clear();
    couts.clear();

    for (int i = 0; i < n; i++) {
        if (matADJ[i].size() != n) {
            qDebug() << "Erreur : ligne " << i << " de matADJ a une taille incorrecte (" << matADJ[i].size() << " au lieu de " << n << ")\n";
            return;
        }

        for (int j = 0; j < n; j++) {
            if (matADJ[i][j] != PAS_DE_LIEN) {
                m_total++;
            }
        }
    }

    fs.resize(n + m_total + 1, 0);
    couts.resize(n + m_total + 1, 0);
    aps.resize(n + 1, 0);

    fs[0] = n + m_total;
    couts[0] = n + m_total;
    aps[0] = n;

    int fs_indice = 1;
    for (int j = 0; j < n; j++) {
        aps[j + 1] = fs_indice;

        for (int i = 0; i < n; i++) {
            if (matADJ[j][i] != PAS_DE_LIEN) {
                fs[fs_indice] = i + 1;
                couts[fs_indice] = matADJ[j][i];
                fs_indice++;
            }
        }

        fs[fs_indice] = 0; // marque la fin des successeurs du sommet j
        fs_indice++;
    }
}

void graphe::det_aps_from_fs(vector<int> fs, vector<int>& aps) const
{

    int nb_sommets=0;
    for (int i=1; i<=fs[0]; i++)
        if(fs[i]==0)
            nb_sommets++;
    aps.resize(nb_sommets+1);
    aps[0]=nb_sommets;
    int j=1;
    aps[j]=1;
    for (int i=1; i<fs[0]; i++)
        if(fs[i]==0)
        {
            j++;
            aps[j]=i+1;
        }

}

void graphe::afficherMatriceAdjacence() const
{
    // Afficher la première ligne (seulement 2 éléments)
    qDebug() << n << m;

    /*// Afficher le reste de la matrice (de 1 à n)
    for (int i = 0; i <getNbSommets(); i++)   // De 1 à n (nb de sommets)
    {
        for (int j = 0; j <getNbSommets(); j++)   // n colonnes
        {
            qDebug() << matADJ[i][j] << " ";
        }
        qDebug() << "\n";
    }*/

    for (const auto& row : matADJ)
    {
        QString ligne;
        for (int val : row)
        {
            ligne += QString::number(val) + " ";
        }
        qDebug() << ligne;
    }
    qDebug() << "\n";
}

/*
bool graphe::estOriente() const
{
    int *ddi ;
    int *dde ;
    int *fs, *aps;
    det_fs_aps(fs, aps);
    det_ddi(aps,fs,ddi);
    det_dde(aps,fs,dde);
    for(int i = 0 ; i<= aps[0]; i++)
    {
        if(ddi[i]!=dde[i])
        {
            return true;
        }

    }
    return false;
}
*/

vector<vector<int>> graphe::getMadj() const
{
    return matADJ;
}
vector<sommet> graphe::getSommets() const
{
    return sommets;
}

void graphe::clearGraphe()
{
    sommets.clear();
    matADJ.clear();
    this->n=0;
    this->m=0;
}

int graphe::getNbSommets() const
{
    return this->n;
}
int graphe::getNbLiens() const
{
    return this->m;
}

int graphe::calculNbLiens() const
{
    int m=0;

    for (int i = 0; i <getNbSommets(); i++)   // De 1 à n (nb de sommets)
    {
        for (int j = 0; j <getNbSommets(); j++)   // n colonnes
        {
            if(matADJ[i][j]!=PAS_DE_LIEN)
                m++;
        }
    }
    return m;
}

void graphe::afficherSommets()
{
    for (int i = 0; i < getNbSommets(); ++i)
    {
        qDebug() << sommets[i].toString()<< " ";
    }
}

void graphe::setSommet(int id, sommet s)
{
    s.id=sommets[id-1].id;
    sommets[id-1]=s;
}

void graphe::setIdDuSommet(int index, int newId)
{
    if (index >= 0 && index < sommets.size()) {
        sommets[index].id = newId; //Permet d'assigner un nouvel ID au sommet à l'index passé en paramètres
    } else {
        qDebug() << "Index hors limites";
    }
}

int graphe::getPoidsDe(lien l) const
{
    return matADJ[l.a.id-1][l.b.id-1];
}
/*
void graphe::setPoidsDe(lien l)
{
    matADJ[l.a][l.b]=l.poids;
}
*/
sommet graphe::getSommet(int id) const
{
    return sommets[id-1];
}

void graphe::ajouterSommet(const sommet& s)
{
    this->n++; // Incrémente le nombre de sommets
    sommets.push_back(s); // Ajoute le sommet au vecteur

    // Ajuster la taille de la matrice d'adjacence sans perdre les données existantes
    for (auto &ligne : matADJ)
    {
        ligne.resize(getNbSommets(), PAS_DE_LIEN); // Ajouter une colonne
    }
    matADJ.push_back(std::vector<int>(getNbSommets(), PAS_DE_LIEN)); // Ajouter une nouvelle ligne

    // Mettre à jour l'ID du sommet ajouté
    sommets.back().id = getNbSommets();
}

void graphe::supprimerSommet(int id)
{
    int n=getNbSommets();
    this->n--;
    for (int i = id-1; i < n-1; ++i)
    {
        sommets[i]=sommets[i+1];
        sommets[i].id=i+1;
    }
    sommets.pop_back();

    for (int i = id-1 ; i < n-1; ++i)
    {
        matADJ[i]=matADJ[i+1];
    }
    matADJ.pop_back();
    //suppression de la derniere ligne

    for (int i = 0; i < n-1; ++i)
    {
        for (int j = id-1; j < n-1; ++j)
        {
            matADJ[i][j]=matADJ[i][j+1];
        }
    }

    n=getNbSommets();

    for (int i = 0; i < n; ++i)
    {
        matADJ[i].pop_back();  // Supprimer la derniere colonne à chaque ligne
    }
    this->m=calculNbLiens();
}

bool graphe::aDesPoidsNegatifs() const
{
    for (int i=0; i< getNbSommets(); i++)
    {

        for (int j=0; j< getNbSommets(); j++)
        {
            if ( matADJ[i][j]!=PAS_DE_LIEN && matADJ[i][j]<0 ) return true;
        }

    }
    return false;
}

void graphe::ecrireLeGrapheDansFichier(const std::string& nomFichierOUT) const {
    // Créer le répertoire "graphes" s'il n'existe pas
    std::filesystem::create_directories("graphes");

    // Ouvrir le fichier dans le répertoire courant + extension ou chemin absolu à donner
    std::fstream fOUT("graphes/" + nomFichierOUT + ".txt", std::ios::out);
    fOUT << getNbSommets()<<" "<< getNbLiens()<< std::endl;

    for (int i=0; i< getNbSommets(); i++)   // De 1 à n (nb de sommets)
    {
        for (int j = 0; j <getNbSommets(); j++)   // n colonnes
        {
            fOUT << matADJ[i][j] <<" ";
        }
        fOUT <<std::endl;
    }
    //ecriture de chaque sommet, a rendre plus generique
    for (int i=0; i< getNbSommets(); i++)   // De 1 à n (nb de sommets)
    {
        sommets[i].ecrireDansFichier(fOUT);
        fOUT <<endl;
    }
    fOUT.close();
}

void graphe::inverserDeuxSommets(int idA, int idB)
{
    //changer les lignes dans la matrice d'adjacence
    //changer les id
    int i=sommets[idA-1].id;
    sommets[idA-1].id=sommets[idB-1].id;
    sommets[idB-1].id=i;
    //changer les structures
    sommet s=sommets[idA-1];
    sommets[idA-1]=sommets[idB-1];
    sommets[idB-1]=s;

    //changer les lignes dans la matrice d'adjacence
    vector<int> ligneA=matADJ[idA-1];
    matADJ[idA-1]=matADJ[idB-1];
    matADJ[idB-1]=ligneA;
}
>>>>>>> 8ce54f0 (commit projet)
