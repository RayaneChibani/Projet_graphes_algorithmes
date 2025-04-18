#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "graphe.h"
#include "graphenonoriente.h"
#include "grapheoriente.h"
#include "affichagegraphe.h"
#include "algorithme.h"

#include <QMainWindow>
#include <QFormLayout>
#include <QTableWidget>
#include <QMessageBox>
#include <QVBoxLayout>
#include <QDir>
#include <unordered_set>

using std::vector;

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void homeWidget();
    void pageAlgos();
    void pageChargerGrapheCourant();
    void clearGraphe();
    void ouvrirDialogGestionSommets();
    void pageSaisieSommetsWidget();
    void pageSaisieLiensWidget();
    void pageVisuGraphe();
    void pageVisuGrapheCourant();
    void pageVisuResultatAlgo();
    void pageExecDistances();
    void pageExecArticulationIsthmes();
    void pageExecDantzig();
    void pageExecKruskal();

private slots:

    void on_btnCharger_clicked();

    void on_btnRetourChargerGraphe_clicked();

    void on_btnAlgo_clicked();

    void on_btnRetourAlgo_clicked();

    void on_btnRetourSaisieLiens_clicked();

    void on_btnSaisie_clicked();

    void on_btnAjouterSommet_clicked();

    void on_btnTerminerSommet_clicked();

    void on_btnRetourSaisieSommets_clicked();

    void on_btnAjouterLien_clicked();

    void on_btnTerminerLiens_clicked();

    void on_btnRetourVisuGraphe_clicked();

    void on_btnSaveGraphe_clicked();

    void on_btnChoisirGrapheCourant_clicked();

    void on_btnGererSommets_clicked();

    void on_btnDistances_clicked();

    void on_btnRang_clicked();

    void on_btnTarjan_clicked();

    void on_btnPtsArti_clicked();

    void on_btnOrdonnancement_clicked();

    void on_btnDijkstra_clicked();

    void on_btnDantzig_clicked();

    void on_btnKruskal_clicked();

    void on_btnChoisirGrapheCourant_2_clicked();

    void on_btnRetourVisuGraphe_2_clicked();

    void on_btnExecAlgo_clicked();

    void on_btnRetourVisuAlgo_clicked();

    void on_btnRetourExecKruskal_clicked();

    void on_btnRetourVisuGraphe_3_clicked();

    void on_btnRetourExecDantzig_clicked();

    void on_btnTaches_clicked();

    void on_btnRetourExecDistances_clicked();

    void on_btnRetourExecArticulationIsthmes_clicked();

    void on_btnVoirGrapheReduit_clicked();

    void on_btnRetourTarjanReduit_clicked();

private:
    Ui::MainWindow *ui;

    class graphe* graphe = nullptr;
    class graphe* grapheCourant = nullptr;
    QString fichierGrapheCourant = "";
    bool grapheOriente = false;
    bool grapheCourantOriente = false;
    int compteurID = 1;  // ID unique pour chaque sommet
    bool grapheCourantEstValue();
    QVector<QString> nomsTaches;  // Tableau pour stocker les noms des tâches pour ordonnancement
    int algoAExecuter = -1;
    void transformerCouts(const std::vector<int>& couts, std::vector<int>& d);
    void afficher_tableau(const char* nom, vector<int> tab);
    void afficherTableauDansLabel(const QString& nomTableau, const vector<int>& tableau, QString& result);
    vector<int> fsr, apsr, prem, pilch;

    class affichagegraphe* affichagegraphe = nullptr;
};
#endif // MAINWINDOW_H
