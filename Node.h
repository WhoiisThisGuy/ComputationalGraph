#ifndef NODE_H
#define NODE_H
#include <QSTring>
#include <QGraphicsItem>
#include <QPainter>
#include <QPen>
#include <QGraphicsSceneMouseEvent>
#include "edge.h"
#include <QQueue>

class Node : public QGraphicsEllipseItem
{
public:
    Node();
    ~Node();
    static const int diameter;

    unsigned int getId() const{return my_Id;}

    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
    void addEdge(Edge*);
    void addChild(Node*);
    void addParent(Node*);
    void getComputableChilds(QQueue<Node*>&);

    bool hasConnection(Node* n) const;
    bool orphan()const;
    bool hasFamily()const;
    bool checkForEdgesInQueue(const QQueue<Node*>& n);

    void removeEdges();

    QVariant itemChange(GraphicsItemChange change, const QVariant &value) override;
private:
    static unsigned int nextNodeId;
    unsigned int my_Id;

    QList<Edge*> my_Edges;
    QList<Node*> my_Parents;
    QList<Node*> my_Childs;
};

#endif // NODE_H
