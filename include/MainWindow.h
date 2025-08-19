#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget> // 100% must be; as heritage
#include "Graph.h"



class QGraphicsView;
class QPushButton;
class QGraphicsScene;
class QGraphicsItemGroup;

// No need Forward Declaration for Graph because using by value


class MainWindow : public QWidget {
    Q_OBJECT

public:
    MainWindow(QWidget* parent = nullptr);

private slots:
    void onLoadButtonClicked();
    void onShowHideButtonClicked();

private:
    void drawGraph();
    void drawPath();
    void clearScene();

    QGraphicsScene* scene;
    QGraphicsView* view;
    QGraphicsItemGroup* graphGroup;
    QGraphicsItemGroup* pathGroup;
    QPushButton* btnLoad;
    QPushButton* btnShowPath;
    
    Graph g;
};

#endif // MAINWINDOW_H
