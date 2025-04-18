#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(ui->editTxtNomSommet, &QLineEdit::returnPressed, this, &MainWindow::on_btnAjouterSommet_clicked);
    connect(ui->editTxtValeurLien, &QLineEdit::returnPressed, this, &MainWindow::on_btnAjouterLien_clicked);

    affichagegraphe = new class affichagegraphe;

    ui->btnTaches->setVisible(false);
    ui->btnVoirGrapheReduit->setVisible(false);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::homeWidget()
{
    ui->stackedWidget->setCurrentWidget(ui->home);
}

void MainWindow::pageChargerGrapheCourant()
{
    ui->stackedWidget->setCurrentWidget(ui->pageChargerGraphe);
}

void MainWindow::pageAlgos()
{
    ui->stackedWidget->setCurrentWidget(ui->pageAlgos);
}

void MainWindow::pageSaisieSommetsWidget()
{
    if (graphe->getSommets().empty())
    {
        ui->btnGererSommets->setVisible(false);
    }
    ui->stackedWidget->setCurrentWidget(ui->pageSaisieSommets);
}

void MainWindow::on_btnRetourSaisieLiens_clicked()
{
    pageSaisieSommetsWidget();
}

void MainWindow::pageSaisieLiensWidget()
{
    ui->stackedWidget->setCurrentWidget(ui->pageSaisieLiens);
}

void MainWindow::pageVisuGraphe()
{
    ui->stackedWidget->setCurrentWidget(ui->pageVisuGraphe);
}

void MainWindow::pageVisuGrapheCourant()
{
    ui->stackedWidget->setCurrentWidget(ui->pageVisuGrapheCourant);
}

void MainWindow::pageVisuResultatAlgo()
{
    ui->stackedWidget->setCurrentWidget(ui->pageVisuResultatAlgos);
}

void MainWindow::pageExecDistances()
{
    ui->stackedWidget->setCurrentWidget(ui->pageExecDistances);
}

void MainWindow::pageExecArticulationIsthmes()
{
    ui->stackedWidget->setCurrentWidget(ui->pageExecArticulationIsthmes);
}

void MainWindow::pageExecDantzig()
{
    ui->stackedWidget->setCurrentWidget(ui->pageExecDantzig);
}

void MainWindow::on_btnRetourExecDantzig_clicked()
{
    affichagegraphe = nullptr;
    ui->graphicsViewExecDantzig->setScene(nullptr);
    pageVisuResultatAlgo();
}

void MainWindow::pageExecKruskal()
{
    ui->stackedWidget->setCurrentWidget(ui->pageExecKruskal);
}

void MainWindow::on_btnCharger_clicked()
{
    QDir repertoire("graphes/");

    //Crée le répertoire s'il n'existe pas
    if (!repertoire.exists()) {
        QDir().mkpath("graphes/");
    }

    // Filtrer les fichiers du répertoire
    QStringList fichiers = repertoire.entryList(QDir::Files);

    // Boucle pour parcourir chaque fichier et les ajouter à labelFichiers
    QString nomsFichiers;
    for (const QString& fichier : fichiers) {
        nomsFichiers += "- " + fichier + "\n"; // Ajouter le nom du fichier à la liste
    }

    QLabel* labelFichiers = new QLabel(this);

    labelFichiers->setText(nomsFichiers);
    labelFichiers->setTextInteractionFlags(Qt::TextSelectableByMouse);
    labelFichiers->setCursor(Qt::IBeamCursor);

    // Ajouter dynamiquement le QLabel dans la scrollArea
    ui->scrollAreaFichiers->setWidget(labelFichiers);
    ui->scrollAreaFichiers->setWidgetResizable(true);

    pageChargerGrapheCourant();
}

void MainWindow::on_btnChoisirGrapheCourant_clicked()
{
    if (ui->editTxtFichier->text() != "")
    {
        QDir repertoire("graphes/");
        QStringList fichiers = repertoire.entryList(QDir::Files);
        bool fichierExiste = false;
        for (const QString& fichier : fichiers)
        {
            if(ui->editTxtFichier->text() == fichier)
            {
                fichierExiste = true;
            }
        }
        if (fichierExiste)
        {
            QMessageBox::information(nullptr, "Succès", "Le graphe du fichier '" + ui->editTxtFichier->text() + "' est désormais courant.");

            fichierGrapheCourant =  ui->editTxtFichier->text();

            QString text = "Graphe courant actuel : ";
            text += fichierGrapheCourant;

            ui->labelFichierCourant->setText(text);
            QPalette palette = ui->labelFichierCourant->palette();
            palette.setColor(QPalette::WindowText, Qt::green);  // Couleur du texte en vert
            ui->labelFichierCourant->setPalette(palette);

            std::ifstream fichier("graphes/" + ui->editTxtFichier->text().toStdString());

            if (fichier.is_open()) {
                int nbSommets, nbLiens;
                fichier >> nbSommets >> nbLiens;

                std::vector<std::vector<int>> matADJCourant(nbSommets, std::vector<int>(nbSommets));

                // Sauter la première ligne (contenant nbSommets et nbLiens) et commencer à lire la matrice à partir de la ligne suivante
                // Lire la matrice d'adjacence à partir du fichier
                for (int i = 0; i < nbSommets; i++) {
                    for (int j = 0; j < nbSommets; j++) {
                        fichier >> matADJCourant[i][j];  // Lire les éléments de la matrice d'adjacence
                    }
                }

                grapheCourantOriente = false;
                for (int i = 0; i < nbSommets; i++) {
                    for (int j = i + 1; j < nbSommets; j++) { // On parcourt seulement la moitié supérieure
                        if (matADJCourant[i][j] != matADJCourant[j][i]) {
                            grapheCourantOriente = true;
                            break;
                        }
                    }
                    if (grapheCourantOriente) break; // Stop dès qu'on sait que c'est orienté
                }

                if (!grapheCourantOriente) {
                    grapheCourant = new class grapheNonOriente("graphes/" + ui->editTxtFichier->text().toStdString());
                    qDebug() << "Graphe non oriente créé" << '\n';
                } else {
                    grapheCourant = new class grapheOriente("graphes/" + ui->editTxtFichier->text().toStdString());
                    qDebug() << "Graphe oriente créé" << '\n';
                }

                grapheCourant->afficherSommets();
                grapheCourant->afficherMatriceAdjacence();

                ui->editTxtFichier->clear();

            } else {
                QMessageBox::warning(nullptr, "Erreur", "Impossible d'ouvrir le fichier.");
            }


        } else {
            QMessageBox::warning(nullptr, "Erreur", "Le fichier entré n'existe pas.");
        }
    } else
    {
        QMessageBox::warning(nullptr, "Erreur", "Veuillez entrer un nom de fichier.");
    }
}

void MainWindow::on_btnRetourChargerGraphe_clicked()
{
    homeWidget();
    ui->editTxtFichier->clear();
}

void MainWindow::on_btnAlgo_clicked()
{
    pageAlgos();
}

void MainWindow::on_btnRetourAlgo_clicked()
{
    homeWidget();
}

void MainWindow::clearGraphe()
{
    ui->editTxtNomSommet->clear();
    ui->editTxtValeurLien->clear();
    graphe->clearGraphe();
    compteurID = 1;
    homeWidget();
}

void MainWindow::on_btnSaisie_clicked()
{
    QDialog dialog(this);
    dialog.setWindowTitle("Type de graphe");

    QVBoxLayout layout(&dialog);

    QLabel label("Veuillez sélectionner le type de graphe à créer :", &dialog);
    layout.addWidget(&label);

    QVBoxLayout* buttonLayout = new QVBoxLayout();

    QPushButton btnOriente("Graphe Orienté", &dialog);
    QPushButton btnNonOriente("Graphe Non Orienté", &dialog);
    QPushButton btnAnnuler("Annuler", &dialog);

    buttonLayout->addWidget(&btnOriente);
    buttonLayout->addWidget(&btnNonOriente);
    buttonLayout->addWidget(&btnAnnuler);

    layout.addLayout(buttonLayout); // Ajouter les boutons dans le layout

    dialog.setLayout(&layout);

    std::vector<std::vector<int>> matADJ;
    std::vector<sommet> sommets;

    // Logique des boutons
    QObject::connect(&btnOriente, &QPushButton::clicked, [&]() {
        qDebug() << "L'utilisateur a choisi un graphe orienté.";
        grapheOriente = true;
        graphe = new class grapheOriente(matADJ, sommets);
        pageSaisieSommetsWidget();
        dialog.accept();
    });

    QObject::connect(&btnNonOriente, &QPushButton::clicked, [&]() {
        qDebug() << "L'utilisateur a choisi un graphe non orienté.";
        grapheOriente = false;
        graphe = new class grapheNonOriente(matADJ, sommets);
        pageSaisieSommetsWidget();
        dialog.accept();
    });

    QObject::connect(&btnAnnuler, &QPushButton::clicked, &dialog, &QDialog::reject);

    dialog.exec();
}

void MainWindow::on_btnGererSommets_clicked()
{
    ouvrirDialogGestionSommets();
}

void MainWindow::ouvrirDialogGestionSommets() {
    QDialog dialog(this);
    dialog.setWindowTitle("Modifier / Supprimer un sommet");

    QVBoxLayout layout(&dialog);

    QLabel label("Choisissez un sommet à modifier ou supprimer :", &dialog);
    layout.addWidget(&label);

    QComboBox comboBox(&dialog);
    // Ajouter les sommets à la ComboBox
    for (const auto& sommet : graphe->getSommets()) {
        comboBox.addItem(QString::fromStdString(sommet.nom));
    }
    layout.addWidget(&comboBox);

    QLineEdit editNom(&dialog);
    layout.addWidget(&editNom);

    // Initialiser l'editNom avec le nom du premier sommet
    if (!graphe->getSommets().empty()) {
        editNom.setText(QString::fromStdString(graphe->getSommets().at(0).nom));
    }

    QPushButton btnModifier("Modifier", &dialog);
    QPushButton btnSupprimer("Supprimer", &dialog);
    QPushButton btnAnnuler("Annuler", &dialog);

    layout.addWidget(&btnModifier);
    layout.addWidget(&btnSupprimer);
    layout.addWidget(&btnAnnuler);

    // Mettre à jour le QLineEdit à chaque fois que l'utilisateur change de sommet dans la ComboBox
    QObject::connect(&comboBox, &QComboBox::currentTextChanged, [&editNom](const QString& text) {
        editNom.setText(text);
    });

    // Logique du bouton Modifier
    QObject::connect(&btnModifier, &QPushButton::clicked, [&]() {
        sommet s;
        s.id = comboBox.currentIndex();
        s.nom = editNom.text().toStdString();
        graphe->setSommet(comboBox.currentIndex() + 1, s);
        QMessageBox::information(nullptr, "Succès", "Sommet modifié avec succès.");
        graphe->afficherSommets();
        graphe->afficherMatriceAdjacence();
        dialog.accept();
    });

    // Logique du bouton Supprimer
    QObject::connect(&btnSupprimer, &QPushButton::clicked, [&]() {
        graphe->supprimerSommet(comboBox.currentIndex() + 1);
        QMessageBox::information(nullptr, "Succès", "Sommet supprimé avec succès.");
        graphe->afficherSommets();
        graphe->afficherMatriceAdjacence();
        dialog.accept();
        if (graphe->getSommets().empty())
        {
            ui->btnGererSommets->setVisible(false);
        }
    });

    // Logique du bouton Annuler
    QObject::connect(&btnAnnuler, &QPushButton::clicked, &dialog, &QDialog::reject);

    dialog.exec();
}

void MainWindow::on_btnAjouterSommet_clicked()
{
    QString nomSommet = ui->editTxtNomSommet->text();

    if (nomSommet != "")
    {
        sommet nouveauSommet;
        nouveauSommet.id = compteurID++;
        nouveauSommet.nom = nomSommet.toStdString();

        graphe->ajouterSommet(nouveauSommet);

        graphe->afficherSommets();

        graphe->afficherMatriceAdjacence();

        QMessageBox::information(this, "Succès", "Sommet ajouté.", QMessageBox::Ok);
        ui->editTxtNomSommet->clear();
        ui->btnGererSommets->setVisible(true);
    } else
    {
        QMessageBox::warning(nullptr, "Erreur", "Veuillez entrer un nom de sommet.");
    }
}

void MainWindow::on_btnTerminerSommet_clicked()
{
    if (graphe->getSommets().empty())
    {
        QMessageBox::warning(this, "Erreur", "Aucun sommet ajouté. Veuillez en ajouter avant de terminer.");
    } else
    {
        QMessageBox msgBox;
        msgBox.setWindowTitle("Confirmation");
        msgBox.setText("Avez-vous terminé de saisir les sommets de ce graphe ?");

        QPushButton *oui = msgBox.addButton("Oui", QMessageBox::AcceptRole);
        msgBox.addButton("Non", QMessageBox::RejectRole);

        msgBox.exec();

        if (msgBox.clickedButton() == oui) {
            ui->comboBoxSommet1->clear();
            ui->comboBoxSommet2->clear();

            for (const auto& sommet : graphe->getSommets())
            {
                QString item = QString::fromStdString(sommet.nom);

                ui->comboBoxSommet1->addItem(item);
                ui->comboBoxSommet2->addItem(item);
            }
            graphe->det_degres();
            pageSaisieLiensWidget();
            ui->editTxtNomSommet->clear();
        }
    }
}


void MainWindow::on_btnRetourSaisieSommets_clicked()
{
    if (!graphe->getMadj().empty())
    {
        QMessageBox msgBox;
        msgBox.setWindowTitle("Confirmation");
        msgBox.setText("Êtes-vous sûr de vouloir revenir au menu ? Les sommets et liens seront perdus.");

        QPushButton *oui = msgBox.addButton("Oui", QMessageBox::AcceptRole);
        msgBox.addButton("Non", QMessageBox::RejectRole);

        msgBox.exec();

        if (msgBox.clickedButton() == oui) {
            clearGraphe();
        }
    } else
    {
        homeWidget();
    }
}

void MainWindow::on_btnAjouterLien_clicked()
{
    graphe->afficherMatriceAdjacence();

    int s1Index = ui->comboBoxSommet1->currentIndex();
    int s2Index = ui->comboBoxSommet2->currentIndex();

    if (s1Index != s2Index)
    {
        if (ui->editTxtValeurLien->text() != "")
        {
            if (ui->editTxtValeurLien->text().toInt(nullptr) != 0 || ui->editTxtValeurLien->text() == "0" || ui->editTxtValeurLien->text() == "MAX")
            {
                int poids;

                if (ui->editTxtValeurLien->text() == "MAX")
                {
                    poids = INT_MAX;
                } else
                {
                    poids = ui->editTxtValeurLien->text().toInt();
                }

                sommet s1 = graphe->getSommets()[s1Index];
                sommet s2 = graphe->getSommets()[s2Index];

                for (int i = 0; i < graphe->getNbSommets(); i++)
                {
                    if (graphe->getMadj()[s1.id - 1][s2.id - 1] != INT_MAX)
                    {
                        if (graphe->getMadj()[s1.id - 1][s2.id - 1] == poids)
                        {
                            QMessageBox::warning(nullptr, "Erreur", "Ce lien existe déjà avec le même poids.");
                            return;
                        }
                        else
                        {
                            if (poids != INT_MAX)
                            {
                                //MAJ du lien
                                lien l = {s1, s2, poids};
                                qDebug() << "Modification lien de" << l.a.nom << "à" <<  l.b.nom << "avec poids" << poids;
                                graphe->setPoidsDe(l);

                                QMessageBox::information(nullptr, "Succès", "Le poids du lien a été mis à jour.");

                                graphe->afficherMatriceAdjacence();

                                return;
                            } else
                            {
                                //Suppression du lien
                                lien l = {s1, s2, poids};
                                qDebug() << "Suppression lien de" << l.a.nom << "à" <<  l.b.nom;
                                graphe->setPoidsDe(l);

                                QMessageBox::information(nullptr, "Succès", "Le lien a été supprimé.");

                                graphe->afficherMatriceAdjacence();

                                return;
                            }
                        }
                    }
                }

                if (poids != INT_MAX)
                {
                    //Si le lien n'existe pas, on l'ajoute
                    lien l = {s1, s2, poids};
                    qDebug() << "Ajout lien de" << l.a.nom << "à" <<  l.b.nom << "avec poids" << poids;
                    graphe->ajouterLien(l);

                    QMessageBox::information(nullptr, "Succès", "Le lien a été ajouté avec succès.");

                    graphe->afficherMatriceAdjacence();
                } else
                {
                    QMessageBox::warning(nullptr, "Erreur", "Impossible de supprimer un lien inexistant.");

                    graphe->afficherMatriceAdjacence();
                }
            }
            else
            {
                QMessageBox::warning(nullptr, "Erreur", "Veuillez entrer un entier.");
            }
        } else
        {
            QMessageBox::warning(nullptr, "Erreur", "Veuillez entrer la valeur du lien.");
        }
    }
    else
    {
        QMessageBox::warning(nullptr, "Erreur", "Veuillez sélectionner deux sommets différents.");
    }
}

void MainWindow::on_btnTerminerLiens_clicked()
{
    QMessageBox msgBox;
    msgBox.setWindowTitle("Confirmation");
    msgBox.setText("Avez-vous terminé de saisir les liens de ce graphe ?");

    QPushButton *oui = msgBox.addButton("Oui", QMessageBox::AcceptRole);
    msgBox.addButton("Non", QMessageBox::RejectRole);

    msgBox.exec();

    if (msgBox.clickedButton() == oui) {
        pageVisuGraphe();

        QGraphicsScene* scene = new QGraphicsScene(this);

        //Dessiner le graphe dans la scène
        affichagegraphe->dessinerGraphe(graphe, grapheOriente, scene);

        //Configurer la vue avec la nouvelle scène
        ui->graphicsView->setScene(scene);

        //Ajuster la vue pour afficher toute la scène
        ui->graphicsView->fitInView(scene->sceneRect(), Qt::KeepAspectRatio);

        //Afficher la vue
        ui->graphicsView->setVisible(true);
    }
}


void MainWindow::on_btnRetourVisuGraphe_clicked()
{
    clearGraphe();
    affichagegraphe = nullptr;
    ui->graphicsView->setScene(nullptr);
    ui->editTxtFichier1->clear();
}

void MainWindow::on_btnRetourVisuGraphe_3_clicked()
{
    pageSaisieLiensWidget();
}


void MainWindow::on_btnSaveGraphe_clicked()
{
    if (ui->editTxtFichier1->text() != "")
    {
        graphe->ecrireLeGrapheDansFichier(ui->editTxtFichier1->text().toStdString());
        QMessageBox::warning(nullptr, "Succès", "Le fichier '" + ui->editTxtFichier1->text() + ".txt' a été sauvegardé dans le répertoire 'graphes'.");
        ui->editTxtFichier1->clear();
    } else
    {
        QMessageBox::warning(nullptr, "Erreur", "Veuillez entrer un nom de fichier.");
    }
}

bool MainWindow::grapheCourantEstValue()
{
    bool grapheCourantValue = false;
    for (int i = 0; i < grapheCourant->getNbSommets(); ++i) {
        for (int j = 0; j < grapheCourant->getNbSommets(); ++j) {
            if (grapheCourant->getMadj()[i][j] != INT_MAX && grapheCourant->getMadj()[i][j] != 1) {
                grapheCourantValue = true; // Si un lien n'est pas égal à 1, le graphe est valué
                break;
            }
        }
        if (grapheCourantValue) break; // On arrête dès qu'on trouve un lien valué
    }

    return grapheCourantValue;
}

void MainWindow::on_btnDistances_clicked()
{
    QDialog dialog(this);
    dialog.setWindowTitle("Distances");

    QVBoxLayout layout(&dialog);

    QLabel label("L'algorithme des distances permet de calculer la distance minimale (en nombre d’arêtes)\n"
                 "entre un sommet de départ et tous les autres sommets d’un graphe.\n"
                 "Il repose sur un parcours en largeur, qui explore le graphe niveau par niveau.\n\n"
                 "Le graphe courant doit être non valué pour pouvoir exécuter l'algorithme des distances.\n", &dialog);

    QFont font;
    font.setPointSize(10);
    label.setFont(font);

    layout.addWidget(&label);

    QPushButton btnExec("Exécuter l'algorithme", &dialog);
    QPushButton btnAnnuler("Annuler", &dialog);

    layout.addWidget(&btnExec);
    layout.addWidget(&btnAnnuler);

    QObject::connect(&btnExec, &QPushButton::clicked, [&]() {
        if (grapheCourant != nullptr && !grapheCourantEstValue())
        {
            //QMessageBox::information(nullptr, "Succès", "Le graphe courant choisi n'est pas valué.");
            algoAExecuter = 1;

            pageVisuResultatAlgo();

            QGraphicsScene* scene = new QGraphicsScene(this);

            //Dessiner le graphe dans la scène
            affichagegraphe->dessinerGraphe(grapheCourant, grapheCourantOriente, scene);

            //Configurer la vue avec la nouvelle scène
            ui->graphicsViewVisuAlgo->setScene(scene);

            //Ajuster la vue pour afficher toute la scène
            ui->graphicsViewVisuAlgo->fitInView(scene->sceneRect(), Qt::KeepAspectRatio);

            //Afficher la vue
            ui->graphicsViewVisuAlgo->setVisible(true);
        } else
        {
            QMessageBox::warning(nullptr, "Erreur", "Veuillez choisir un graphe courant non valué.");
        }
        dialog.accept();
    });

    QObject::connect(&btnAnnuler, &QPushButton::clicked, &dialog, &QDialog::reject);

    dialog.exec();
}

void MainWindow::on_btnRang_clicked()
{
    QDialog dialog(this);
    dialog.setWindowTitle("Rang");

    QVBoxLayout layout(&dialog);

    QLabel label("L'algorithme du rang permet de calculer le rang d’un sommet dans un graphe orienté.\n"
                 "Il évalue la position d'un sommet dans le graphe en fonction du nombre d'arêtes\n"
                 "qui pointent vers lui (ddi) ou du nombre d'arêtes qui partent de lui (dde).\n\n"
                 "Le graphe courant doit être orienté non valué pour pouvoir exécuter l'algorithme du rang.\n", &dialog);

    QFont font;
    font.setPointSize(10);
    label.setFont(font);

    layout.addWidget(&label);

    QPushButton btnExec("Exécuter l'algorithme", &dialog);
    QPushButton btnAnnuler("Annuler", &dialog);

    layout.addWidget(&btnExec);
    layout.addWidget(&btnAnnuler);

    QObject::connect(&btnExec, &QPushButton::clicked, [&]() {
        if (grapheCourant != nullptr && !grapheCourantEstValue() && grapheCourantOriente)
        {
            //QMessageBox::information(nullptr, "Succès", "Le graphe courant choisi est orienté non valué.");

            algoAExecuter = 2;

            pageVisuResultatAlgo();

            QGraphicsScene* scene = new QGraphicsScene(this);

            //Dessiner le graphe dans la scène
            affichagegraphe->dessinerGraphe(grapheCourant, grapheCourantOriente, scene);

            //Configurer la vue avec la nouvelle scène
            ui->graphicsViewVisuAlgo->setScene(scene);

            //Ajuster la vue pour afficher toute la scène
            ui->graphicsViewVisuAlgo->fitInView(scene->sceneRect(), Qt::KeepAspectRatio);

            //Afficher la vue
            ui->graphicsViewVisuAlgo->setVisible(true);
        } else
        {
            QMessageBox::warning(nullptr, "Erreur", "Veuillez choisir un graphe courant orienté non valué.");
        }
        dialog.accept();
    });

    QObject::connect(&btnAnnuler, &QPushButton::clicked, &dialog, &QDialog::reject);

    dialog.exec();
}

void MainWindow::on_btnTarjan_clicked()
{
    QDialog dialog(this);
    dialog.setWindowTitle("Tarjan");

    QVBoxLayout layout(&dialog);

    QLabel label("L'algorithme de Tarjan est un algorithme utilisé pour trouver les composantes fortement connexes (CFC)\n"
                 "et le graphe réduit dans un graphe orienté.\n"
                 "Une composante fortement connexe est un sous-graphe dans lequel chaque sommet est accessible\n"
                 "depuis tous les autres sommets du sous-graphe.\n"
                 "Le graphe réduit est une version condensée du graphe initial, obtenue en regroupant les CFC en un seul sommet.\n\n"
                 "Le graphe courant doit être orienté non valué pour pouvoir exécuter l'algorithme de tarjan.\n", &dialog);

    QFont font;
    font.setPointSize(10);
    label.setFont(font);

    layout.addWidget(&label);

    QPushButton btnExec("Exécuter l'algorithme", &dialog);
    QPushButton btnAnnuler("Annuler", &dialog);

    layout.addWidget(&btnExec);
    layout.addWidget(&btnAnnuler);

    QObject::connect(&btnExec, &QPushButton::clicked, [&]() {
        if (grapheCourant != nullptr && !grapheCourantEstValue() && grapheCourantOriente)
        {
            //QMessageBox::information(nullptr, "Succès", "Le graphe courant choisi est orienté non valué.");

            algoAExecuter = 3;

            ui->labelCFC->clear();

            ui->btnVoirGrapheReduit->setVisible(false);

            pageVisuResultatAlgo();

            QGraphicsScene* scene = new QGraphicsScene(this);

            //Dessiner le graphe dans la scène
            affichagegraphe->dessinerGraphe(grapheCourant, grapheCourantOriente, scene);

            //Configurer la vue avec la nouvelle scène
            ui->graphicsViewVisuAlgo->setScene(scene);

            //Ajuster la vue pour afficher toute la scène
            ui->graphicsViewVisuAlgo->fitInView(scene->sceneRect(), Qt::KeepAspectRatio);

            //Afficher la vue
            ui->graphicsViewVisuAlgo->setVisible(true);
        } else
        {
            QMessageBox::warning(nullptr, "Erreur", "Veuillez choisir un graphe courant orienté non valué.");
        }
        dialog.accept();
    });

    QObject::connect(&btnAnnuler, &QPushButton::clicked, &dialog, &QDialog::reject);

    dialog.exec();
}

void MainWindow::on_btnPtsArti_clicked()
{
    QDialog dialog(this);
    dialog.setWindowTitle("Points d'articulation et isthmes");

    QVBoxLayout layout(&dialog);

    QLabel label("Un point d'articulation est un sommet qui, s'il est retiré avec toutes ses arêtes, divise le graphe\n"
                 "en plusieurs composantes fortement connexes.\n\n"
                 "Un isthme est une arête dont la suppression augmente le nombre de composantes connexes dans le graphe.\n"
                 "C'est une arête essentielle pour la connectivité entre certains sommets.\n\n"
                 "Le graphe courant doit être orienté pour pouvoir exécuter la détermination des points d'articulation et isthmes.\n", &dialog);

    QFont font;
    font.setPointSize(10);
    label.setFont(font);

    layout.addWidget(&label);

    QPushButton btnExecPtsArti("Exécuter l'algorithme de détermination des points d'articulation", &dialog);
    QPushButton btnExecIsthme("Exécuter l'algorithme de détermination des isthmes", &dialog);
    QPushButton btnAnnuler("Annuler", &dialog);

    layout.addWidget(&btnExecPtsArti);
    layout.addWidget(&btnExecIsthme);
    layout.addWidget(&btnAnnuler);

    QObject::connect(&btnExecPtsArti, &QPushButton::clicked, [&]() {
        if (grapheCourant != nullptr && !grapheCourantOriente)
        {
            //QMessageBox::information(nullptr, "Succès", "Le graphe courant choisi est non orienté.");

            algoAExecuter = 4;

            pageVisuResultatAlgo();

            QGraphicsScene* scene = new QGraphicsScene(this);

            //Dessiner le graphe dans la scène
            affichagegraphe->dessinerGraphe(grapheCourant, grapheCourantOriente, scene);

            //Configurer la vue avec la nouvelle scène
            ui->graphicsViewVisuAlgo->setScene(scene);

            //Ajuster la vue pour afficher toute la scène
            ui->graphicsViewVisuAlgo->fitInView(scene->sceneRect(), Qt::KeepAspectRatio);

            //Afficher la vue
            ui->graphicsViewVisuAlgo->setVisible(true);
        } else
        {
            QMessageBox::warning(nullptr, "Erreur", "Veuillez choisir un graphe courant non orienté.");
        }
        dialog.accept();
    });

    QObject::connect(&btnExecIsthme, &QPushButton::clicked, [&]() {
        if (grapheCourant != nullptr && !grapheCourantOriente)
        {
            //QMessageBox::information(nullptr, "Succès", "Le graphe courant choisi est non orienté.");

            algoAExecuter = 5;

            pageVisuResultatAlgo();

            QGraphicsScene* scene = new QGraphicsScene(this);

            //Dessiner le graphe dans la scène
            affichagegraphe->dessinerGraphe(grapheCourant, grapheCourantOriente, scene);

            //Configurer la vue avec la nouvelle scène
            ui->graphicsViewVisuAlgo->setScene(scene);

            //Ajuster la vue pour afficher toute la scène
            ui->graphicsViewVisuAlgo->fitInView(scene->sceneRect(), Qt::KeepAspectRatio);

            //Afficher la vue
            ui->graphicsViewVisuAlgo->setVisible(true);
        } else
        {
            QMessageBox::warning(nullptr, "Erreur", "Veuillez choisir un graphe courant non orienté.");
        }
        dialog.accept();
    });

    QObject::connect(&btnAnnuler, &QPushButton::clicked, &dialog, &QDialog::reject);

    dialog.exec();
}

void MainWindow::on_btnOrdonnancement_clicked()
{
    QDialog dialog(this);
    dialog.setWindowTitle("Ordonnancement");

    QVBoxLayout layout(&dialog);

    QLabel label("L'algorithme de l'ordonnancement est utilisé pour déterminer la durée minimale d'un projet,\n"
                 "en respectant l'ordre et la durée de toutes les tâches de ce projet.\n\n"
                 "Le graphe courant doit être orienté valué pour pouvoir exécuter l'algorithme de l'ordonnancement.\n", &dialog);

    QFont font;
    font.setPointSize(10);
    label.setFont(font);

    layout.addWidget(&label);

    QPushButton btnExec("Exécuter l'algorithme", &dialog);
    QPushButton btnAnnuler("Annuler", &dialog);

    layout.addWidget(&btnExec);
    layout.addWidget(&btnAnnuler);

    QObject::connect(&btnExec, &QPushButton::clicked, [&]() {
        if (grapheCourant != nullptr && grapheCourantEstValue() && grapheCourantOriente) {
            QDialog saisieDialog(this);
            saisieDialog.setWindowTitle("Saisie des noms de tâches");

            QVBoxLayout* layout = new QVBoxLayout(&saisieDialog);
            QFormLayout* formLayout = new QFormLayout();

            QVector<QLineEdit*> champsTaches;
            int nbSommets = grapheCourant->getNbSommets();

            for (int i = 0; i < nbSommets; ++i) {
                QString nomSommet = QString::fromStdString(grapheCourant->getSommet(i+1).nom);
                QLineEdit* champ = new QLineEdit(&saisieDialog);
                champ->setPlaceholderText(nomSommet);
                champ->setFixedWidth(300);
                formLayout->addRow(QString("Nom de la tâche %1 :").arg(nomSommet), champ);
                champsTaches.append(champ);
            }

            layout->addLayout(formLayout);
            QPushButton* btnValider = new QPushButton("Valider");
            layout->addWidget(btnValider);

            QObject::connect(btnValider, &QPushButton::clicked, [&]() {
                // Réinitialiser nomsTaches avant d'y ajouter de nouveaux noms
                nomsTaches.clear();

                for (QLineEdit* champ : champsTaches) {
                    if (!champ->text().isEmpty()) {
                        nomsTaches.append(champ->text());
                    }
                }

                qDebug() << "Tâches saisies : " << nomsTaches;

                // Code pour l'algorithme ou autre action ici
                algoAExecuter = 6;
                pageVisuResultatAlgo();

                QGraphicsScene* scene = new QGraphicsScene(this);
                affichagegraphe->dessinerGraphe(grapheCourant, grapheCourantOriente, scene);

                ui->graphicsViewVisuAlgo->setScene(scene);
                ui->graphicsViewVisuAlgo->fitInView(scene->sceneRect(), Qt::KeepAspectRatio);
                ui->graphicsViewVisuAlgo->setVisible(true);
                ui->btnTaches->setVisible(true);

                saisieDialog.accept();
                dialog.accept();  // ferme aussi la première boîte
            });

            saisieDialog.exec();
        } else {
            QMessageBox::warning(nullptr, "Erreur", "Veuillez choisir un graphe courant orienté valué.");
        }
    });
    QObject::connect(&btnAnnuler, &QPushButton::clicked, &dialog, &QDialog::reject);

    dialog.exec();
}

void MainWindow::on_btnDijkstra_clicked()
{
    QDialog dialog(this);
    dialog.setWindowTitle("Dijkstra");

    QVBoxLayout layout(&dialog);

    QLabel label("L'algorithme de Dijkstra est utilisé pour trouver le plus court chemin entre un sommet de départ\n"
                 "et tous les autres sommets dans un graphe orienté valué (où les arêtes ont des poids non négatifs).\n\n"
                 "Le graphe courant doit être orienté valué pour pouvoir exécuter l'algorithme de Dijkstra.\n", &dialog);

    QFont font;
    font.setPointSize(10);
    label.setFont(font);

    layout.addWidget(&label);

    QPushButton btnExec("Exécuter l'algorithme", &dialog);
    QPushButton btnAnnuler("Annuler", &dialog);

    layout.addWidget(&btnExec);
    layout.addWidget(&btnAnnuler);

    QObject::connect(&btnExec, &QPushButton::clicked, [&]() {
        if (grapheCourant != nullptr && grapheCourantEstValue() && !grapheCourant->aDesPoidsNegatifs() && grapheCourantOriente)
        {
            //QMessageBox::information(nullptr, "Succès", "Le graphe courant choisi est orienté valué.");
            algoAExecuter = 7;

            pageVisuResultatAlgo();

            QGraphicsScene* scene = new QGraphicsScene(this);

            //Dessiner le graphe dans la scène
            affichagegraphe->dessinerGraphe(grapheCourant, grapheCourantOriente, scene);

            //Configurer la vue avec la nouvelle scène
            ui->graphicsViewVisuAlgo->setScene(scene);

            //Ajuster la vue pour afficher toute la scène
            ui->graphicsViewVisuAlgo->fitInView(scene->sceneRect(), Qt::KeepAspectRatio);

            //Afficher la vue
            ui->graphicsViewVisuAlgo->setVisible(true);
        } else
        {
            QMessageBox::warning(nullptr, "Erreur", "Veuillez choisir un graphe courant orienté valué ayant des poids non négatifs.");
        }
        dialog.accept();
    });

    QObject::connect(&btnAnnuler, &QPushButton::clicked, &dialog, &QDialog::reject);

    dialog.exec();
}

void MainWindow::on_btnDantzig_clicked()
{
    QDialog dialog(this);
    dialog.setWindowTitle("Dantzig");

    QVBoxLayout layout(&dialog);

    QLabel label("L'algorithme de Dantzig résout le problème des plus courts chemins dans un graphe en utilisant une approche\n"
                 "par extensions successives. Contrairement à Dijkstra, l'algorithme de Dantzig accepte les poids négatifs.\n\n"
                 "Le graphe courant doit être orienté valué pour pouvoir exécuter l'algorithme de Dantzig.\n", &dialog);

    QFont font;
    font.setPointSize(10);
    label.setFont(font);

    layout.addWidget(&label);

    QPushButton btnExec("Exécuter l'algorithme", &dialog);
    QPushButton btnAnnuler("Annuler", &dialog);

    layout.addWidget(&btnExec);
    layout.addWidget(&btnAnnuler);

    QObject::connect(&btnExec, &QPushButton::clicked, [&]() {
        if (grapheCourant != nullptr && grapheCourantEstValue() && grapheCourantOriente)
        {
            //QMessageBox::information(nullptr, "Succès", "Le graphe courant choisi est orienté valué.");

            pageVisuResultatAlgo();

            QGraphicsScene* scene = new QGraphicsScene(this);

            //Dessiner le graphe dans la scène
            affichagegraphe->dessinerGraphe(grapheCourant, grapheCourantOriente, scene);

            //Configurer la vue avec la nouvelle scène
            ui->graphicsViewVisuAlgo->setScene(scene);

            //Ajuster la vue pour afficher toute la scène
            ui->graphicsViewVisuAlgo->fitInView(scene->sceneRect(), Qt::KeepAspectRatio);

            //Afficher la vue
            ui->graphicsViewVisuAlgo->setVisible(true);

            algoAExecuter = 8;
        } else
        {
            QMessageBox::warning(nullptr, "Erreur", "Veuillez choisir un graphe courant orienté valué.");
        }
        dialog.accept();
    });

    QObject::connect(&btnAnnuler, &QPushButton::clicked, &dialog, &QDialog::reject);

    dialog.exec();
}

void MainWindow::on_btnKruskal_clicked()
{
    QDialog dialog(this);
    dialog.setWindowTitle("Kruskal");

    QVBoxLayout layout(&dialog);

    QLabel label("L'algorithme de Kruskal permet de déterminer l'arbre recouvrant de poids minimal dans un graphe valué non orienté.\n"
                 "Il commence par trier les arêtes du graphe en fonction de leurs poids, puis les ajoute une à une à l'arbre résultat,\n"
                 "à condition que l'ajout de l'arête ne crée pas de cycle.\n\n"
                 "Le graphe courant doit être valué non orienté pour pouvoir exécuter l'algorithme de Kruskal.\n", &dialog);

    QFont font;
    font.setPointSize(10);
    label.setFont(font);

    layout.addWidget(&label);

    QPushButton btnExec("Exécuter l'algorithme", &dialog);
    QPushButton btnAnnuler("Annuler", &dialog);

    layout.addWidget(&btnExec);
    layout.addWidget(&btnAnnuler);

    QObject::connect(&btnExec, &QPushButton::clicked, [&]() {
        if (grapheCourant != nullptr && grapheCourantEstValue() && !grapheCourantOriente)
        {
            //QMessageBox::information(nullptr, "Succès", "Le graphe courant choisi est valué non orienté.");

            algoAExecuter = 9;

            pageVisuResultatAlgo();

            QGraphicsScene* scene = new QGraphicsScene(this);

            //Dessiner le graphe dans la scène
            affichagegraphe->dessinerGraphe(grapheCourant, grapheCourantOriente, scene);

            //Configurer la vue avec la nouvelle scène
            ui->graphicsViewVisuAlgo->setScene(scene);

            //Ajuster la vue pour afficher toute la scène
            ui->graphicsViewVisuAlgo->fitInView(scene->sceneRect(), Qt::KeepAspectRatio);

            //Afficher la vue
            ui->graphicsViewVisuAlgo->setVisible(true);
        } else
        {
            QMessageBox::warning(nullptr, "Erreur", "Veuillez choisir un graphe courant valué non orienté.");
        }
        dialog.accept();
    });

    QObject::connect(&btnAnnuler, &QPushButton::clicked, &dialog, &QDialog::reject);

    dialog.exec();
}

void MainWindow::on_btnChoisirGrapheCourant_2_clicked()
{
    if (grapheCourant != nullptr)
    {
        pageVisuGrapheCourant();

        QGraphicsScene* scene = new QGraphicsScene(this);

        //Dessiner le graphe dans la scène
        affichagegraphe->dessinerGraphe(grapheCourant, grapheCourantOriente, scene);

        //Configurer la vue avec la nouvelle scène
        ui->graphicsView_2->setScene(scene);

        //Ajuster la vue pour afficher toute la scène
        ui->graphicsView_2->fitInView(scene->sceneRect(), Qt::KeepAspectRatio);

        //Afficher la vue
        ui->graphicsView_2->setVisible(true);
    } else
    {
        QMessageBox::warning(nullptr, "Erreur", "Veuillez charger un graphe courant avant d'essayer de le visualiser.");
    }
}


void MainWindow::on_btnRetourVisuGraphe_2_clicked()
{
    pageChargerGrapheCourant();
    affichagegraphe = nullptr;
    ui->graphicsView_2->setScene(nullptr);
}

void MainWindow::afficherTableauDansLabel(const QString& nomTableau, const vector<int>& tableau, QString& result)
{
    result += nomTableau + " : [";

    // Traiter chaque élément du tableau
    for (int i = 0; i < tableau.size(); ++i) {
        if (tableau[i] == INT_MAX) {
            result += "∞";  // Remplace INT_MAX par le symbole ∞
        } else {
            result += QString::number(tableau[i]);
        }

        // Ajouter une virgule pour séparer les éléments, mais pas après le dernier
        if (i != tableau.size() - 1) {
            result += ", ";  // Ajout de la virgule entre les éléments
        }
    }

    result += "]\n";  // Ajout d'un saut de ligne après chaque tableau
}

void MainWindow::transformerCouts(const std::vector<int>& couts, std::vector<int>& d) {
    d.push_back(grapheCourant->getNbSommets());

    bool newGroup = true;  // Utilisé pour déterminer quand un nouveau groupe commence
    int prevVal = -1;

    for (int i = 1; i < couts.size(); ++i) {
        if (couts[i] != 0) {
            // Si ce n'est pas 0 et qu'il y a un changement de valeur, on l'ajoute
            if (newGroup || couts[i] != prevVal) {
                d.push_back(couts[i]);
                prevVal = couts[i];
                newGroup = false;  // On a ajouté une valeur, ce n'est plus un nouveau groupe
            }
        } else {
            // Si c'est un 0, on marque la fin du groupe actuel
            newGroup = true;
        }
    }
}

void MainWindow::afficher_tableau(const char* nom, vector<int> tab) {
    cout << nom << " : ";
    // Utilisation de tab.size() pour éviter d'utiliser tab[0] comme taille
    for (int i = 0; i < tab.size(); i++) {
        cout << tab[i] << " ";
    }
    cout << endl;
}

void MainWindow::on_btnExecAlgo_clicked()
{
    algorithme a;

    //-----------------------------------------------DISTANCES
    if (algoAExecuter == 1)
    {
        vector<int> aps, fs, couts;
        grapheCourant->det_fs_aps_couts(fs, aps, couts);
        vector<vector<int>> matDistances = a.distances(aps, fs);

        pageExecDistances();

        QGraphicsScene* scene = new QGraphicsScene(this);

        //Dessiner le graphe dans la scène
        affichagegraphe->dessinerGraphe(grapheCourant, grapheCourantOriente, scene);

        //Configurer la vue avec la nouvelle scène
        ui->graphicsViewExecDistances->setScene(scene);

        //Ajuster la vue pour afficher toute la scène
        ui->graphicsViewExecDistances->fitInView(scene->sceneRect(), Qt::KeepAspectRatio);

        //Afficher la vue
        ui->graphicsViewExecDistances->setVisible(true);

        vector<vector<int>> copieMatrice = grapheCourant->getMadj();

        int n = copieMatrice.size();
        for (int i = 0; i < n; ++i) {
            if (i < copieMatrice[i].size()) {
                copieMatrice[i][i] = 0;
            }
        }

        vector<vector<int>> copieMatriceAvant = grapheCourant->getMadj();

        QString matriceAvant = "Matrice avant exécution :\n\n";

        // Parcours de la matrice et construction de la chaîne
        for (const auto& row : copieMatriceAvant) {
            for (int cost : row) {
                if (cost >= INT_MAX) {
                    matriceAvant += "∞   ";
                } else {
                    matriceAvant += QString::number(cost) + "   ";
                }
            }
            matriceAvant += "\n";
        }

        // Assignation de la chaîne au QLabel
        ui->labelMatDistances1->setText(matriceAvant);

        // --------------------------------------------- Affichage matrice après

        QString matriceApres = "Matrice après exécution :\n\n";

        for (size_t i = 1; i < matDistances.size(); ++i) {
            for (size_t j = 1; j < matDistances[i].size(); ++j) {
                if (matDistances[i][j] == -1)
                {
                    matriceApres += "∞   ";
                } else {
                    matriceApres += QString::number(matDistances[i][j]) + "   ";
                }
            }
            matriceApres += "\n"; // Retour à la ligne après chaque ligne de la matrice
        }

        // Assignation de la chaîne au QLabel
        ui->labelMatDistances2->setText(matriceApres);
    }

    //-----------------------------------------------RANG
    if (algoAExecuter == 2)
    {
        vector<int> aps, fs, couts;
        vector<int> rang, prem, pilch;

        grapheCourant->det_fs_aps_couts(fs, aps, couts);

        a.rang(fs, aps, rang, prem, pilch);

        QString result;

        afficherTableauDansLabel("prem", prem, result);
        afficherTableauDansLabel("pilch", pilch, result);
        afficherTableauDansLabel("rang", rang, result);

        ui->labelResultatsAlgo->setText(result);
    }

    //-----------------------------------------------TARJAN
    if (algoAExecuter == 3)
    {
        vector<int> aps, fs, couts, cfc, pred;

        grapheCourant->det_fs_aps_couts(fs, aps, couts);

        a.fortconnexe(aps, fs, prem, pilch, cfc, pred);

        afficher_tableau("aps", aps);
        afficher_tableau("fs", fs);

        afficher_tableau("prem", prem);
        afficher_tableau("pilch", pilch);
        afficher_tableau("cfc", cfc);
        afficher_tableau("pred", pred);

        QString result;

        afficherTableauDansLabel("prem", prem, result);
        afficherTableauDansLabel("pilch", pilch, result);
        afficherTableauDansLabel("cfc", cfc, result);

        ui->labelResultatsAlgo->setText(result);

        QString tarjanString = "Composantes fortement connexes :\n";

        bool premiereComposante = true; // Utilisé pour éviter d'ajouter une virgule avant la première composante
        for (int i = 1; i < cfc.size(); ++i) {
            // Vérifier si la composante est non vide
            bool composanteNonVide = false;
            QString composante;
            composante += "C" + QString::number(i) + ": {";
            // Afficher les sommets de la composante fortement connexe
            for (int sommet = 1; sommet < cfc.size(); ++sommet) {
                if (cfc[sommet] == i) {
                    composante += QString::number(sommet) + ", ";
                    composanteNonVide = true;
                }
            }
            // Si la composante est vide, ne pas l'afficher
            if (!composanteNonVide) {
                continue;
            }
            composante.chop(2); // Supprimer la virgule et l'espace après le dernier sommet
            composante += "}";
            if (!premiereComposante) {
                tarjanString += ", ";
            } else {
                premiereComposante = false;
            }
            tarjanString += composante;
        }

        ui->labelCFC->setText(tarjanString);

        a.grapheReduit(prem, pilch, cfc, fs, aps, fsr, apsr);

        afficher_tableau("fsr", fsr);
        afficher_tableau("apsr", apsr);

        if (apsr[0] != 0 && fsr[0] != 0) {
            ui->btnVoirGrapheReduit->setVisible(true);
            ui->labelCFC_2->setText(tarjanString);
            ui->labelResultatsAlgo_2->setText(ui->labelResultatsAlgo->text());
        } else {
            ui->btnVoirGrapheReduit->setVisible(false);
            tarjanString += " (pas de graphe réduit)";
        }
    }

    //-----------------------------------------------PTS D'ARTICULATION
    if (algoAExecuter == 4)
    {
        vector<sommet> ptsArti;
        grapheNonOriente* grNonOriente = dynamic_cast<grapheNonOriente*>(grapheCourant);

        a.det_points_articulation(*grNonOriente, ptsArti);

        // Afficher les points d'articulation
        qDebug() << "Points d'articulation :";
        for (const sommet& s : ptsArti) {
            qDebug() << s.toString();
        }

        QString text;

        if (ptsArti.empty())
        {
            text += "Aucun point d'articulation";
        } else {
            text += "Points d'articulation :\n\n";

            for (const sommet& s : ptsArti) {
                text += "- " + s.nom + "\n";
            }
        }

        pageExecArticulationIsthmes();

        QGraphicsScene* scene = new QGraphicsScene(this);

        //Dessiner le graphe dans la scène
        affichagegraphe->dessinerGraphe(grapheCourant, grapheCourantOriente, scene);

        //Configurer la vue avec la nouvelle scène
        ui->graphicsViewExecArticulationIsthmes->setScene(scene);

        //Ajuster la vue pour afficher toute la scène
        ui->graphicsViewExecArticulationIsthmes->fitInView(scene->sceneRect(), Qt::KeepAspectRatio);

        //Afficher la vue
        ui->graphicsViewExecArticulationIsthmes->setVisible(true);

        ui->labelExecArticulationIsthmes->setText(text);
    }

    //-----------------------------------------------ISTHMES
    if (algoAExecuter == 5)
    {
        vector<lien> isthmes;
        grapheNonOriente* grNonOriente = dynamic_cast<grapheNonOriente*>(grapheCourant);

        a.det_isthmes(*grNonOriente, isthmes);

        // Afficher les isthmes
        qDebug() << "Isthmes :";
        for (const lien& l : isthmes) {
            qDebug() << "Arête : " << l.a.nom << " -> " << l.b.nom;
        }

        QString text;

        if (isthmes.empty())
        {
            text += "Aucun isthme";
        } else {
            text += "Isthmes :\n\n";
            for (const lien& l : isthmes) {
                text += "- " + l.a.nom + " -> " + l.b.nom + "\n";  // Affiche les noms des sommets des liens
            }
        }

        pageExecArticulationIsthmes();

        QGraphicsScene* scene = new QGraphicsScene(this);

        //Dessiner le graphe dans la scène
        affichagegraphe->dessinerGraphe(grapheCourant, grapheCourantOriente, scene);

        //Configurer la vue avec la nouvelle scène
        ui->graphicsViewExecArticulationIsthmes->setScene(scene);

        //Ajuster la vue pour afficher toute la scène
        ui->graphicsViewExecArticulationIsthmes->fitInView(scene->sceneRect(), Qt::KeepAspectRatio);

        //Afficher la vue
        ui->graphicsViewExecArticulationIsthmes->setVisible(true);

        ui->labelExecArticulationIsthmes->setText(text);
    }

    //-----------------------------------------------ORDONNANCEMENT
    if (algoAExecuter == 6)
    {
        vector<int> fs, aps, couts;
        grapheCourant->det_fs_aps_couts(fs, aps, couts);

        vector<int> ddi, app, fp;

        int n = aps[0];
        ddi.resize(n+1);
        ddi[0] = n;
        for(int s = 1; s <= n; s++)
            ddi[s] = 0;
        for(int k = 1; k < fs[0]; k++)
            if (fs[k] !=0)
                ddi[fs[k]] ++;

        grapheCourant->det_app(aps, ddi, app);
        grapheCourant->det_fp(fs, aps, app, fp);

        std::vector<int> d;

        // Transformer les couts en d
        transformerCouts(couts, d);

        vector<int> fpc, appc, lc;

        a.ordonnancement(fp, app, d, fpc, appc, lc);

        QString result;

        afficherTableauDansLabel("appc", appc, result);
        afficherTableauDansLabel("fpc", fpc, result);
        afficherTableauDansLabel("lc", lc, result);

        ui->labelResultatsAlgo->setText(result);

        vector<int> apsc, fsc;

        std::vector<int> dde(n + 1, 0); // degré sortant (nb de successeurs) de chaque sommet

        // 1. Compter le nombre de successeurs (en lisant les prédécesseurs)
        for (int s = 1; s <= n; ++s) {
            for (int i = appc[s]; fpc[i] != 0; ++i) {
                int p = fpc[i]; // p est un prédécesseur de s => s est un successeur de p
                dde[p]++;
            }
        }

        // 2. Construire aps à partir du tableau des degrés sortants
        apsc.resize(n + 1);
        apsc[0] = n;
        apsc[1] = 1;
        for (int i = 1; i < n; ++i) {
            apsc[i + 1] = apsc[i] + dde[i] + 1;
        }

        // 3. Initialiser fs avec la bonne taille
        fsc.resize(apsc[n] + 1);
        fsc[0] = apsc[n];

        // 4. Préparer un tableau de positions courantes d'insertion
        std::vector<int> current(apsc); // copie de aps

        // 5. Parcourir les prédécesseurs et remplir fs
        for (int s = 1; s <= n; ++s) {
            for (int i = appc[s]; fpc[i] != 0; ++i) {
                int p = fpc[i];
                fsc[current[p]++] = s; // s est successeur de p
            }
        }

        // 6. Ajouter les zéros de terminaison
        for (int i = 1; i <= n; ++i) {
            fsc[current[i]] = 0;
        }

        vector<int> coutsc;

        coutsc.resize(fsc.size(), 0);

        for (int s = 1; s <= n; ++s) {
            for (int i = apsc[s]; fsc[i] != 0; ++i) {
                int succ = fsc[i];

                // Chercher dans le graphe d'origine le coût entre s -> succ
                int coutArc = 0;

                for (int j = aps[s]; fs[j] != 0; ++j) {
                    if (fs[j] == succ) {
                        coutArc = couts[j];  // On a trouvé le bon arc
                        break;
                    }
                }

                coutsc[i] = coutArc;
            }
        }

        afficher_tableau("apsc", apsc);
        afficher_tableau("fsc", fsc);
        afficher_tableau("coutsc", coutsc);

        vector<sommet> nouveauxSommets = grapheCourant->getSommets();

        class graphe* grapheReduit = nullptr;
        grapheReduit = new class grapheOriente(fsc, apsc, coutsc, nouveauxSommets);
        grapheReduit->afficherMatriceAdjacence();

        QGraphicsScene* scene = new QGraphicsScene(this);

        //Dessiner le graphe dans la scène
        affichagegraphe->dessinerGraphe(grapheReduit, grapheCourantOriente, scene);

        //Configurer la vue avec la nouvelle scène
        ui->graphicsViewVisuAlgo->setScene(scene);

        //Ajuster la vue pour afficher toute la scène
        ui->graphicsViewVisuAlgo->fitInView(scene->sceneRect(), Qt::KeepAspectRatio);

        //Afficher la vue
        ui->graphicsViewVisuAlgo->setVisible(true);
    }

    //-----------------------------------------------DIJKSTRA
    if (algoAExecuter == 7)
    {
        QDialog dialog(this);
        dialog.setWindowTitle("Sommet de départ");

        QVBoxLayout layout(&dialog);

        QLabel label("Choisissez le sommet de départ :", &dialog);
        layout.addWidget(&label);

        QComboBox comboBox(&dialog);
        // Ajouter les sommets à la ComboBox
        for (const auto& sommet : grapheCourant->getSommets()) {
            comboBox.addItem(QString::fromStdString(sommet.nom));
        }
        layout.addWidget(&comboBox);

        QPushButton btnConfirmer("Confirmer", &dialog);
        layout.addWidget(&btnConfirmer);

        // Logique du bouton Modifier
        QObject::connect(&btnConfirmer, &QPushButton::clicked, [&]() {
            int s = comboBox.currentIndex() + 1;
            vector<int> d, pr;
            a.dijkstra(*static_cast<class grapheOriente*>(grapheCourant), s, d, pr);

            QString result;

            afficherTableauDansLabel("d", d, result);
            afficherTableauDansLabel("p", pr, result);

            ui->labelResultatsAlgo->setText(result);

            // Création du graphe réduit
            vector<vector<int>> matAdjReduite(d.size(), vector<int>(d.size(), PAS_DE_LIEN));
            vector<sommet> nouveauxSommets = grapheCourant->getSommets();

            // On construit le graphe réduit à partir des distances et prédécesseurs
            for (int i = 0; i < d.size(); ++i) {
                // Si la distance est infinie ou s'il n'y a pas de prédécesseur, on passe au sommet suivant
                if (d[i] == INT_MAX || pr[i] == INT_MAX) {
                    continue; // Ce sommet est inaccessible ou n'a pas de prédécesseur valide
                }

                int u = pr[i] - 1;
                int v = i;

                // Assure-toi que u et v sont dans les limites de la matrice d'adjacence
                if (u >= 0 && u < d.size() && v >= 0 && v < d.size()) {
                    int poids = grapheCourant->getMadj()[u][v];
                    if (poids != PAS_DE_LIEN) {
                        matAdjReduite[u][v] = poids;
                    }
                }
            }

            // Création du graphe réduit avec la matrice d'adjacence obtenue
            class graphe* grapheReduit = nullptr;
            grapheReduit = new class grapheOriente(matAdjReduite, nouveauxSommets);
            qDebug() << "Graphe réduit à partir de Dijkstra depuis sommet" << s;
            grapheReduit->afficherMatriceAdjacence();

            QGraphicsScene* scene = new QGraphicsScene(this);

            //Dessiner le graphe dans la scène
            affichagegraphe->dessinerGraphe(grapheReduit, grapheCourantOriente, scene);

            //Configurer la vue avec la nouvelle scène
            ui->graphicsViewVisuAlgo->setScene(scene);

            //Ajuster la vue pour afficher toute la scène
            ui->graphicsViewVisuAlgo->fitInView(scene->sceneRect(), Qt::KeepAspectRatio);

            //Afficher la vue
            ui->graphicsViewVisuAlgo->setVisible(true);

            dialog.accept();
        });

        QPushButton btnAnnuler("Annuler", &dialog);
        layout.addWidget(&btnAnnuler);

        // Logique du bouton Annuler
        QObject::connect(&btnAnnuler, &QPushButton::clicked, &dialog, &QDialog::reject);

        dialog.exec();
    }

    //-----------------------------------------------DANTZIG
    if (algoAExecuter == 8)
    {
        pageExecDantzig();

        QGraphicsScene* scene = new QGraphicsScene(this);

        //Dessiner le graphe dans la scène
        affichagegraphe->dessinerGraphe(grapheCourant, grapheCourantOriente, scene);

        //Configurer la vue avec la nouvelle scène
        ui->graphicsViewExecDantzig->setScene(scene);

        //Ajuster la vue pour afficher toute la scène
        ui->graphicsViewExecDantzig->fitInView(scene->sceneRect(), Qt::KeepAspectRatio);

        //Afficher la vue
        ui->graphicsViewExecDantzig->setVisible(true);

        vector<vector<int>> copieMatrice = grapheCourant->getMadj();

        int n = copieMatrice.size();
        for (int i = 0; i < n; ++i) {
            if (i < copieMatrice[i].size()) {
                copieMatrice[i][i] = 0;
            }
        }

        vector<vector<int>> copieMatriceAvant = copieMatrice;

        if (a.dantzig(copieMatrice))
        {
            QString matriceAvant = "Matrice avant exécution :\n\n";

            // Parcours de la matrice et construction de la chaîne
            for (const auto& row : copieMatriceAvant) {
                for (int cost : row) {
                    if (cost >= INT_MAX) {
                        matriceAvant += "∞   ";
                    } else {
                        matriceAvant += QString::number(cost) + "   ";
                    }
                }
                matriceAvant += "\n";
            }

            // Assignation de la chaîne au QLabel
            ui->labelMatDantzig1->setText(matriceAvant);

            // --------------------------------------------- Affichage matrice après

            QString matriceApres = "Matrice après exécution :\n\n";

            // Parcours de la matrice et construction de la chaîne
            for (const auto& row : copieMatrice) {
                for (int cost : row) {
                    if (cost >= INT_MAX) {
                        matriceApres += "∞   ";
                    } else if (cost <= -INT_MAX) {
                        matriceApres += "-∞   ";
                    } else {
                        matriceApres += QString::number(cost) + "   ";
                    }
                }
                matriceApres += "\n"; // Nouvelle ligne pour chaque ligne de la matrice
            }

            // Assignation de la chaîne au QLabel
            ui->labelMatDantzig2->setText(matriceApres);
        } else
        {
            QMessageBox::warning(nullptr, "Circuit absorbant", "Présence d'un circuit absorbant.\n");
        }
    }

    //-----------------------------------------------KRUSKAL
    if (algoAExecuter == 9)
    {
        vector<lien> aretes;
        a.creerAretes(*grapheCourant, aretes);
        vector<int> prem, pilch, cfc;

        vector<lien> aretesT = a.kruskal(aretes, prem, pilch, cfc);

        vector<vector<int>> matADJ;
        vector<sommet> sommets;

        grapheNonOriente* T = new grapheNonOriente(matADJ, sommets);

        std::unordered_set<int> sommetsAjoutes;
        std::vector<lien> aretesAajouter;
        QString text;

        for (const auto& arete : aretesT) {
            sommet sommetDepart = arete.a;
            sommet sommetArrivee = arete.b;
            int valeur = arete.poids;

            qDebug() << "Ajout de l'arete (" << sommetDepart.nom << ", " << sommetArrivee.nom << ") (valeur = " << valeur << ") au graphe T.";

            text += "Ajout de l'arête (" + QString::fromStdString(sommetDepart.nom) + ", "
                    + QString::fromStdString(sommetArrivee.nom) + ") (valeur = "
                    + QString::number(valeur) + ") à l'arbre T.\n";

            // Ajouter les sommets au graphe T
            if (sommetsAjoutes.find(sommetDepart.id) == sommetsAjoutes.end()) {
                T->ajouterSommet(sommetDepart);
                // Après l'ajout, réassigner l'ID avec la méthode setIdDuSommet
                int index = T->getSommets().size() - 1; // Dernier sommet ajouté
                T->setIdDuSommet(index, sommetDepart.id); // Assigner l'ID correct
                qDebug() << "DEPART : " << sommetDepart.nom << sommetDepart.id;
                sommetsAjoutes.insert(sommetDepart.id);
            }

            if (sommetsAjoutes.find(sommetArrivee.id) == sommetsAjoutes.end()) {
                T->ajouterSommet(sommetArrivee);
                // Après l'ajout, réassigner l'ID avec la méthode setIdDuSommet
                int index = T->getSommets().size() - 1; // Dernier sommet ajouté
                T->setIdDuSommet(index, sommetArrivee.id); // Assigner l'ID correct
                qDebug() << "ARRIVEE : " << sommetArrivee.nom << sommetArrivee.id;
                sommetsAjoutes.insert(sommetArrivee.id);
            }

            // Stocker l'arête pour l'ajouter plus tard
            lien l = {sommetDepart, sommetArrivee, valeur};
            aretesAajouter.push_back(l);
        }

        // Maintenant, réassigner les IDs des sommets ajoutés dans le graphe T
        for (int i = 0; i < T->getSommets().size(); ++i) {
            // Réassigner l'ID basé sur la position dans le vecteur
            T->getSommets()[i].id = i + 1;  // Réattribuer les IDs correctement
            qDebug() << "Sommet " << T->getSommets()[i].nom << " ID " << T->getSommets()[i].id;
        }

        // Affichage
        T->det_degres();
        T->afficherMatriceAdjacence();
        T->afficherSommets();

        // Ajouter toutes les arêtes après avoir ajouté les sommets
        for (const auto& lienAajouter : aretesAajouter) {
            T->ajouterLien(lienAajouter);
        }

        T->afficherSommets();
        T->afficherMatriceAdjacence();

        pageExecKruskal();

        QGraphicsScene* scene = new QGraphicsScene(this);

        //Dessiner le graphe dans la scène
        affichagegraphe->dessinerGraphe(T, grapheCourantOriente, scene);

        //Configurer la vue avec la nouvelle scène
        ui->graphicsViewExecKruskal->setScene(scene);

        //Ajuster la vue pour afficher toute la scène
        ui->graphicsViewExecKruskal->fitInView(scene->sceneRect(), Qt::KeepAspectRatio);

        //Afficher la vue
        ui->graphicsViewExecKruskal->setVisible(true);

        ui->labelExecKruskal->setText(text);
    }
}

void MainWindow::on_btnRetourVisuAlgo_clicked()
{
    affichagegraphe = nullptr;
    ui->labelResultatsAlgo->clear();
    ui->graphicsViewVisuAlgo->setScene(nullptr);
    ui->btnTaches->setVisible(false);
    nomsTaches.clear();
    pageAlgos();
}

void MainWindow::on_btnRetourExecDistances_clicked()
{
    affichagegraphe = nullptr;
    ui->labelMatDistances1->clear();
    ui->labelMatDistances2->clear();
    ui->graphicsViewExecDistances->setScene(nullptr);
    pageVisuResultatAlgo();
}

void MainWindow::on_btnVoirGrapheReduit_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->pageExecTarjanReduit);

    QGraphicsScene* scene = new QGraphicsScene(this);

    vector<int> couts;

    vector<sommet> sommetsReduit;

    int nbCFC = apsr[0];  // Nombre de composantes fortement connexes = nb de sommets dans le graphe réduit

    sommetsReduit.reserve(nbCFC);

    for (int i = 0; i < nbCFC; ++i) {
        sommet s;
        s.id = i + 1;  // Les IDs commencent à 1, pour correspondre à l’indexation des sommets
        s.nom = "C" + std::to_string(i + 1);  // Nommer les sommets selon la composante
        sommetsReduit.push_back(s);
    }

    for (const auto& s : sommetsReduit) {
        qDebug() << QString::fromStdString(s.toString());
    }

    class graphe* grapheReduit = new class grapheOriente(fsr, apsr, couts, sommetsReduit);

    vector<int> br;

    algorithme a;
    a.base_Greduit(apsr, fsr, br);

    QSet<int> basesUniques;
    QStringList basesList;

    for (int i = 1; i <= br[0]; ++i) {
        int base = br[i];
        if (base != 0 && !basesUniques.contains(base)) {
            basesUniques.insert(base);
            basesList << "C" + QString::number(base);
        }
    }

    QString basesStr = "Bases du graphe réduit : {" + basesList.join(", ") + "}";

    vector<vector<int>> bases = a.edition_bases(prem, pilch, br); // Méthode modifiée pour retourner les bases

    basesStr += "\nBases du graphe initial : ";

    for (size_t i = 0; i < bases.size(); ++i) {
        QStringList sommetsStr;
        for (int sommet : bases[i]) {
            sommetsStr << QString::number(sommet);
        }
        basesStr += "B" + QString::number(i + 1) + " : { " + sommetsStr.join(", ") + " }";

        if (i != bases.size() - 1) {
            basesStr += ", ";
        }
    }

    ui->labelBases->setText(basesStr);

    //Dessiner le graphe dans la scène
    affichagegraphe->dessinerGraphe(grapheReduit, grapheCourantOriente, scene);

    //Configurer la vue avec la nouvelle scène
    ui->graphicsViewTarjanReduit->setScene(scene);

    //Ajuster la vue pour afficher toute la scène
    ui->graphicsViewTarjanReduit->fitInView(scene->sceneRect(), Qt::KeepAspectRatio);

    //Afficher la vue
    ui->graphicsViewTarjanReduit->setVisible(true);
}

void MainWindow::on_btnRetourTarjanReduit_clicked()
{
    affichagegraphe = nullptr;
    ui->graphicsViewTarjanReduit->setScene(nullptr);
    pageVisuResultatAlgo();
}

void MainWindow::on_btnRetourExecArticulationIsthmes_clicked()
{
    affichagegraphe = nullptr;
    ui->labelExecArticulationIsthmes->clear();
    ui->graphicsViewExecArticulationIsthmes->setScene(nullptr);
    pageVisuResultatAlgo();
}

void MainWindow::on_btnRetourExecKruskal_clicked()
{
    affichagegraphe = nullptr;
    ui->labelExecKruskal->clear();
    ui->graphicsViewExecKruskal->setScene(nullptr);
    pageVisuResultatAlgo();
}

void MainWindow::on_btnTaches_clicked()
{
    // Créer une nouvelle boîte de dialogue
    QDialog dialog(this);
    dialog.setWindowTitle("Noms des Tâches");

    // Créer un layout vertical pour la fenêtre de dialogue
    QVBoxLayout* layout = new QVBoxLayout(&dialog);

    // Remplir la boîte de dialogue avec les informations des sommets et leurs tâches
    int nbSommets = grapheCourant->getNbSommets();

    for (int i = 0; i < nbSommets; ++i)
    {
        // Récupérer le nom réel du sommet
        QString nomSommet = QString::fromStdString(grapheCourant->getSommet(i+1).nom);

        // Vérifier que l'indice existe dans le tableau des tâches
        QString nomTache = (i < nomsTaches.size()) ? nomsTaches[i] : "";  // Utilise "i" pour l'indexation correcte

        // Créer un label pour afficher le nom du sommet et de la tâche
        QLabel* label = new QLabel(QString("%1 : %2").arg(nomSommet).arg(nomTache), &dialog);
        layout->addWidget(label);  // Ajouter le label au layout
    }

    // Ajouter un bouton pour fermer la fenêtre
    QPushButton* btnFermer = new QPushButton("Fermer", &dialog);
    layout->addWidget(btnFermer);

    // Connecter le bouton "Fermer" à la fermeture de la fenêtre
    QObject::connect(btnFermer, &QPushButton::clicked, &dialog, &QDialog::accept);

    // Exécuter la boîte de dialogue
    dialog.exec();
}
