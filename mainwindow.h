#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFrame>
#include <QPushButton>
#include <QList>
#include <QGraphicsScene>
#include <QGraphicsView>
#include "Node.h"


QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

using namespace std;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void on_Create_Node();

    void on_Delete_Node();

    void sceneSelectionChanged();
    void on_Compute();
private:
    Ui::MainWindow *ui;

    QGraphicsScene scene;
    QGraphicsView view;

    QPushButton* my_button_CreateNode;

    //Pens n Brush
    QPen pen;
    QBrush brush = QBrush(Qt::red);
    QList<Node*> my_Graph;
};
#endif // MAINWINDOW_H
