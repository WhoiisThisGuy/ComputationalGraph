#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QGraphicsItem>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(ui->button_CreateNode, &QPushButton::released, this, &MainWindow::on_Create_Node);
    connect(ui->button_DeleteNode, &QPushButton::released, this, &MainWindow::on_Delete_Node);
    connect(ui->button_Compute, &QPushButton::released, this, &MainWindow::on_Compute);

    view.setScene(&scene);
    view.setParent(this);
    view.show();

    pen = QPen(Qt::black, 10);
    brush = QBrush(Qt::white);

    ui->statusbar->showMessage(tr("Well Hello There!"));
}

void MainWindow::on_Compute()
{
    if(my_Graph.empty())
    {
        ui->statusbar->showMessage("Empty graph no computing.");
        return;
    }

    QQueue<Node*> queue;

    for(auto n : my_Graph)
    {
        if(!n->hasFamily())
        {
            ui->statusbar->showMessage("Not every nodes are connected. No computing.");
            return;
        }
        if(n->orphan())
        {
            queue.enqueue(n);
        }
    }
    ui->wallOfResult->clear();
    int level = 0;
    ui->wallOfResult->append("Level\tNodes");

    QString row;
    int indexOfLastInRow = queue.size()-1;
    int i = 0;
    row.append(QString::number(level)).append("                      ");

    while(true)
    {
        auto n = queue.dequeue();
        n->getComputableChilds(queue);

        row.append(QChar(n->getId()));

        if(i == indexOfLastInRow)
        {
            ui->wallOfResult->append(row);
            if(!queue.empty())
            {
                indexOfLastInRow = queue.size()-1;
                i = 0;
                ++level;
                row.clear();
                row.append(QString::number(level)).append("                      ");
                continue;
            }
            else
            {
                break;
            }
        }
        ++i;
    }
}

void MainWindow::on_Delete_Node()
{
    auto selected_itemz = scene.selectedItems();

    if(selected_itemz.size() == 0)
    {
        return;
    }

    for(auto item : selected_itemz)
    {
        if(item)
        {
            auto textItem = dynamic_cast<QGraphicsTextItem*>(item->childItems().at(0));
            if(textItem)
            {
                delete textItem;
            }
            delete item;
        }
    }
    ui->statusbar->showMessage(tr("Removed item."));
}

void MainWindow::on_Create_Node()
{
    Node * n = new Node();
    if(n->getId()>=90)
    {
        ui->button_CreateNode->setEnabled(false);
    }

    auto t = scene.addText(QChar(n->getId()));

    n->setRect(0,0,Node::diameter,Node::diameter);
    n->setBrush(brush);
    scene.addItem(n);
    n->setFlag(QGraphicsItem::ItemIsMovable);
    n->setFlag(QGraphicsItem::ItemIsSelectable);

    QPointF pf(n->rect().center()*0.5);
    t->setPos(pf);
    t->setParentItem(n);

    my_Graph.append(n);

    ui->statusbar->showMessage(tr("Node created: ").append(QChar(n->getId())));
}


MainWindow::~MainWindow()
{
    delete ui;
}
