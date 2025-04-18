#include "affichagegraphe.h"

affichagegraphe::affichagegraphe() {}

void affichagegraphe::dessinerGraphe(graphe* g, bool oriente, QGraphicsScene* scene)
{
    // Si le graphe est vide, il n'y a rien à dessiner
    if (g->getSommets().empty() && g->getNbLiens() == 0) return;

    // Rayon du cercle
    const int rayon = 100;

    // Centre du cercle
    QPointF centre(scene->width() / 2, scene->height() / 2);

    // Tri des sommets en fonction de leur ID
    std::vector<sommet> sommets = g->getSommets();
    std::sort(sommets.begin(), sommets.end(), [](const sommet& a, const sommet& b) {
        return a.id < b.id;  // Trier par ID croissant
    });

    // Dessiner les sommets
    int i = 0; // Pour garder une trace de l'indice du sommet
    QVector<QPointF> positionsSommets;
    for (const auto& s : sommets) {  // Utiliser le tableau trié
        qreal x = centre.x() + rayon * cos(2 * M_PI * i / sommets.size());
        qreal y = centre.y() + rayon * sin(2 * M_PI * i / sommets.size());
        positionsSommets.push_back(QPointF(x, y));

        // Dessiner le sommet (cercle)
        QGraphicsEllipseItem* sommetItem = scene->addEllipse(x - 10, y - 10, 20, 20,
                                                             QPen(Qt::black), QBrush(Qt::transparent));

        // Ajouter le nom du sommet
        QString nomSommet = QString::fromStdString(s.nom);
        QGraphicsTextItem* nomSommetItem = scene->addText(nomSommet);

        // Mettre le texte en gras
        QFont font = nomSommetItem->font();
        font.setBold(true);
        nomSommetItem->setFont(font);

        // Centrer le texte dans le cercle
        QRectF textRect = nomSommetItem->boundingRect();
        qreal textX = x - textRect.width() / 2;
        qreal textY = y - textRect.height() / 2;
        nomSommetItem->setPos(textX, textY);

        // Ajouter les éléments à la scène
        scene->addItem(sommetItem);
        scene->addItem(nomSommetItem);

        i++;
    }

    // Vérification si le graphe est valué (liens autres que 1)
    bool grapheValue = false;
    for (int i = 0; i < g->getNbSommets(); ++i) {
        for (int j = 0; j < g->getNbSommets(); ++j) {
            if (g->getMadj()[i][j] != INT_MAX && g->getMadj()[i][j] != 1) {
                grapheValue = true; // Si un lien n'est pas égal à 1, le graphe est valué
                break;
            }
        }
        if (grapheValue) break; // On arrête dès qu'on trouve un lien valué
    }

    // Dessiner les liens entre les sommets
    for (int i = 0; i < g->getNbSommets(); ++i) {  // i commence à 1, comme dans la matrice
        for (int j = 0; j < g->getNbSommets(); ++j) {  // j commence à 0, comme dans la matrice
            int poids = g->getMadj()[i][j];

            if (poids != INT_MAX) { // Il y a un lien entre i et j
                qDebug() << "Dessin d'un lien de" << i << "vers" << j << "avec poids" << poids;

                // Vérification que i et j sont dans les limites
                if (i >= positionsSommets.size() || j >= positionsSommets.size()) {
                    qDebug() << "Erreur : accès hors limites dans positionsSommets";
                    continue;
                }

                QPointF start = positionsSommets[i];  // Accéder à positionsSommets[i] pour le sommet i
                QPointF end = positionsSommets[j];        // Accéder à positionsSommets[j] pour le sommet j

                qreal rayonSommet = 10;  // Rayon du cercle représentant un sommet

                // Calcul de la direction du lien
                QPointF direction = end - start;
                qreal longueur = sqrt(direction.x() * direction.x() + direction.y() * direction.y());

                // Normalisation du vecteur direction
                if (longueur > 0) {
                    direction /= longueur;
                }

                // Décaler le début et la fin du segment pour qu'il s'arrête au bord du sommet
                QPointF newStart = start + direction * rayonSommet;
                QPointF newEnd = end - direction * rayonSommet;

                // Dessiner le segment entre les sommets
                QGraphicsLineItem* lienItem = scene->addLine(newStart.x(), newStart.y(), newEnd.x(), newEnd.y(), QPen(Qt::black));

                // Si le graphe est orienté, ajouter une flèche
                if (oriente) {
                    QPolygonF arrowHead;
                    arrowHead << QPointF(4, 0) << QPointF(-4, -2) << QPointF(-4, 2);

                    // Calculer le milieu du segment
                    QPointF midPoint = (start + end) / 2;  // Milieu du segment entre start et end

                    // Déplacer la flèche un peu avant la fin du segment (ex: à 80% du segment)
                    qreal factor = 0.80;  // 80% du chemin entre start et end
                    QPointF arrowPos = start + factor * (end - start);  // Décaler un peu avant la fin

                    // Crée la flèche et la place à cette position
                    QGraphicsPolygonItem* arrow = scene->addPolygon(arrowHead, QPen(Qt::black), QBrush(Qt::black));
                    arrow->setPos(arrowPos);  // Positionner la flèche avant la fin du segment

                    // Calculer la rotation pour orienter correctement la flèche
                    qreal angle = atan2(end.y() - start.y(), end.x() - start.x()) * 180 / M_PI;
                    arrow->setRotation(angle);

                    // Placer le texte juste au-dessus de la flèche
                    if (grapheValue) {
                        QPointF arrowCenter = arrow->mapToScene(arrow->boundingRect().center());
                        QGraphicsTextItem* poidsItem = scene->addText(QString::number(poids));
                        QPointF offset(-5, - 5);  // Décalage à gauche et vers le haut
                        QPointF textPos = arrowCenter + offset;
                        poidsItem->setPos(textPos);
                    }
                }
                // Cas pour les graphes non orientés mais valués
                else if (grapheValue) {
                    // Calculer la position du texte comme pour les graphes orientés
                    QPointF midPoint = (start + end) / 2;  // Milieu du lien pour les graphes non orientés
                    QGraphicsTextItem* poidsItem = scene->addText(QString::number(poids));
                    QPointF offset(- 5, - 5);  // Décalage à gauche et vers le haut
                    QPointF textPos = midPoint + offset;
                    poidsItem->setPos(textPos);
                }
            }
        }
    }
}
