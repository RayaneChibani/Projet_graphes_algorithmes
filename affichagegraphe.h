#ifndef AFFICHAGEGRAPHE_H
#define AFFICHAGEGRAPHE_H

#include "graphe.h"
#include <QGraphicsView>
#include <QString>
#include <QMessageBox>
#include <QGraphicsScene>
#include <QGraphicsEllipseItem>
#include <QGraphicsLineItem>

class affichagegraphe
{
public:
    affichagegraphe();
    void dessinerGraphe(graphe* g, bool oriente, QGraphicsScene* scene);
};

#endif // AFFICHAGEGRAPHE_H
