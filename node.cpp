#include "node.h"
#include <QGraphicsScene>

const int Node::diameter = 30;
unsigned int Node::nextNodeId = 64;

Node::Node()
{
   my_Id = ++nextNodeId;
}

Node::~Node()
{
    int i = 0;
    for(auto e: my_Edges){
        if(e)
        {
            //Deletion of edges remains
            // if(e->from != this)
            // {
            //     e->to->removeEdge(e);
            // }
            // else
            // {
            //     e->from->removeEdge(e);
            // }
            // delete e;
            e->setVisible(false);
        }
        //++i;
    }
}

void Node::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    if(event->buttons() == Qt::MouseButton::LeftButton)
    {
        for(auto e : my_Edges)
        {
            if(e == nullptr || !e->isVisible())
            {
                continue;
            }

            auto l = e->line();
            if(e->from == this)
            {
                l.setP1(QPointF(pos().x()+Node::diameter*0.5,pos().y()+Node::diameter*0.5));
            }
            else
            {
                l.setP2(QPointF(pos().x()+Node::diameter*0.5,pos().y()+Node::diameter*0.5));
            }
            e->setLine(l);
        }
    }
    QGraphicsEllipseItem::mouseMoveEvent(event);
}

void Node::addEdge(Edge * e)
{
    my_Edges.append(e);
}

void Node::addChild(Node * c)
{
    my_Childs.append(c);
}

void Node::addParent(Node * p)
{
    my_Parents.append(p);
}

//Is there an edge between this and any of the nodes that are in the queue
void Node::getComputableChilds(QQueue<Node *> &queue)
{
    for(auto chn : my_Childs)
    {
        if(!chn->checkForEdgesInQueue(queue))
        {
            queue.enqueue(chn);
        }
    }
}

bool Node::hasConnection(Node *n) const
{
    if(my_Childs.indexOf(n) == -1)
    {
        return false;
    }
    return true;
}

bool Node::orphan() const
{
    return my_Parents.empty();
}

bool Node::hasFamily() const
{
    return !my_Edges.empty();
}

bool Node::checkForEdgesInQueue(const QQueue<Node*>& queue)
{
    for(auto qn : queue)
    {
        for(auto e : my_Edges)
        {
            if(e->from == qn)
            {
                return true;
            }
        }
    }
    return false;
}

void Node::removeEdges()
{
    my_Edges.clear();
}

QVariant Node::itemChange(GraphicsItemChange change, const QVariant &value)
{
    if(change == ItemSelectedChange)
    {
        const auto itemz = scene()->selectedItems();

        if( change == ItemSelectedChange && value.toBool() && itemz.size() == 1) //means selected second
        {
            int i = 0;
            Node *fromN = nullptr;
            Node *toN = this;
            for(auto n : itemz)
            {
                if(n!= this)
                {
                    fromN = dynamic_cast<Node*>(n);
                }
                 ++i;
            }
                if(fromN && toN && !fromN->hasConnection(toN))
                {
                    QLineF lf(fromN->pos().x()+Node::diameter*0.5,fromN->pos().y()+Node::diameter*0.5,toN->pos().x()+Node::diameter*0.5,toN->pos().y()+Node::diameter*0.5);
                    // QLineF lfLeft(lf.x1(),lf.y1(),lf.x1()-7,lf.y1()-12);
                    // QLineF lfRight(lf.x1(),lf.y1(),lf.x1()-7,lf.y1()+12);
                    // QGraphicsLineItem* arrLeft = new QGraphicsLineItem(lfLeft);
                    // QGraphicsLineItem* arrRight = new QGraphicsLineItem(lfRight);

                    Edge* edge = new Edge(fromN,toN);
                    // arrLeft->setParentItem(edge);
                    // arrRight->setParentItem(edge);
                    // scene.addItem(arrLeft);
                    // scene.addItem(arrRight);
                    edge->setLine(lf);
                    scene()->addItem(edge);

                    fromN->addEdge(edge);
                    fromN->addChild(toN);
                    fromN->setSelected(false);

                    toN->addParent(fromN);
                    toN->addEdge(edge);
                    toN->setSelected(false);
                }
                else
                {
                    qCritical() << "void MainWindow::sceneSelectionChanged() fromN or toN is nullptr!";
               }
        }
    }
    return QGraphicsItem::itemChange(change, value);
}
