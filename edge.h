#ifndef EDGE_H
#define EDGE_H

#include <QGraphicsLineItem>
class Node;

class Edge : public QGraphicsLineItem
{
public:
    Edge(Node* f, Node* t);
    Node *from;
    Node *to;
    static int sumOfEdges;
};

#endif // EDGE_H
