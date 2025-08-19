#include "MainWindow.h"
#include "AStar.h"
#include "GraphLoader.h"
#include <QGraphicsView>
#include <QGraphicsEllipseItem>
#include <QGraphicsLineItem>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QMessageBox>
#include <QCoreApplication>


MainWindow::MainWindow(QWidget *parent) : QWidget(parent), view(new QGraphicsView(this)),
btnLoad(new QPushButton("READ input.txt", this)), btnShowPath(new QPushButton("SHOW/HIDE", this)),
graphGroup(new QGraphicsItemGroup), pathGroup(new QGraphicsItemGroup),
scene(new QGraphicsScene(this)) {

    QHBoxLayout *mainLayout = new QHBoxLayout(this);
    view->setRenderHint(QPainter::Antialiasing);
    mainLayout->addWidget(view, 1);
    
    QVBoxLayout *buttonLayout = new QVBoxLayout();
    buttonLayout->addWidget(btnLoad);
    buttonLayout->addWidget(btnShowPath);
    buttonLayout->addStretch();
    mainLayout->addLayout(buttonLayout);
    
    connect(btnLoad, &QPushButton::clicked, this, &MainWindow::onLoadButtonClicked);
    connect(btnShowPath, &QPushButton::clicked, this, &MainWindow::onShowHideButtonClicked);
    
    
    scene->setSceneRect(-30, -30, 830, 430);

    view->setScene(scene);
    view->setRenderHint(QPainter::Antialiasing);
    view->setViewportUpdateMode(QGraphicsView::FullViewportUpdate);
    view->setOptimizationFlags(QGraphicsView::DontAdjustForAntialiasing);
        
    
    scene->addItem(graphGroup);
    scene->addItem(pathGroup);
    
}



void MainWindow::onLoadButtonClicked() {
    clearScene();
    QString exePath = QCoreApplication::applicationDirPath();
    QString localPath = exePath + "/input.txt";
    int res = GraphLoader::load(localPath.toStdString(), &g);
    if (res != 0) {
        std::string s = std::string("BAD FILE, code ") + std::to_string(res);
        QMessageBox::information(nullptr,
                                "Error",
                                s.c_str());
    } else {
        g.initInfo();
        g.processBetterVersion(100);
        drawGraph();
        drawPath();
    }
    
    
    
    
}

void MainWindow::onShowHideButtonClicked() {
    
    pathGroup->setVisible(!pathGroup->isVisible());
}

void MainWindow::drawGraph() {
    int numNodes = g.ptrs.size();
    std::vector<std::vector<int>> connections(numNodes, std::vector<int>(numNodes, -1));
    
    for (int v = 0; v < numNodes; v++) {
        Graph::info::Point pv = g.infos[v].p;
        
        QGraphicsEllipseItem *point = new QGraphicsEllipseItem(0, 0, 10, 10);
        point->setPos((int)pv.x, (int)pv.y);
        point->setBrush(Qt::black);
        
        QGraphicsTextItem *label = new QGraphicsTextItem("Point " + QString::number(v));
        label->setParentItem(point);
        
        graphGroup->addToGroup(point);
        
        int bound = ((v == numNodes - 1) ? (g.edges.size()) : (g.ptrs[v + 1]));
        for (int edgeCnt = g.ptrs[v]; edgeCnt < bound; edgeCnt++) {
            int u = g.edges[edgeCnt].first;
            int dist = g.edges[edgeCnt].second;
            connections[v][u] = dist;
            if (connections[u][v] == -1) {
                continue;
            }
            Graph::info::Point pu = g.infos[u].p;
            
            QGraphicsLineItem *line = new QGraphicsLineItem((int)pv.x + 5, (int)pv.y + 5, (int)pu.x + 5, (int)pu.y + 5);
            line->setPen(QPen(Qt::black, 2));
            
            QString txt;
            if (connections[v][u] == connections[u][v]) {
                txt = QString::number(connections[v][u]);
            } else {
                if (v == fmin(u, v)) {
                    txt = "(" + QString::number(connections[v][u]) +  ", " + QString::number(connections[u][v]) + ")";
                } else {
                    txt = "(" + QString::number(connections[u][v]) +  ", " + QString::number(connections[v][u]) + ")";
                }
            }
            
            QGraphicsTextItem *lbl = new QGraphicsTextItem(txt);
            lbl->setParentItem(line);
            
            Graph::info::Point mid = (pv + pu) / 2;
            lbl->setPos((int)mid.x, (int)mid.y );

            graphGroup->addToGroup(line);
        }
    }
}

void MainWindow::drawPath() {
    std::function<int(int, int)> returnZero = [](int a, int b) {return 0;};
    auto v = AStar::findPath(0, g.ptrs.size() - 1, returnZero, g);
    g.lenOfWay = v[v.size() - 1];
    if (!v.empty()) {
        for (int i = 0; i < v.size() - 1; i++) {
            Graph::info::Point p = g.infos[v[i]].p;
            if (i != 0) {
                Graph::info::Point pred = g.infos[v[i - 1]].p;
                QGraphicsLineItem *line = new QGraphicsLineItem((int)pred.x + 5, (int)pred.y + 5, (int)p.x + 5, (int)p.y + 5);
                    line->setPen(QPen(Qt::red, 2));
                    pathGroup->addToGroup(line);
            }
            QGraphicsEllipseItem *point = new QGraphicsEllipseItem(0, 0, 10, 10);
            point->setPos((int)p.x, (int)p.y);
            point->setBrush(Qt::red);
            pathGroup->addToGroup(point);
        }
    } else {
        QMessageBox::information(nullptr,
                                    "Path Finding",
                                    "No path found between nodes!");
    }
}





void MainWindow::clearScene()
{
    auto children = graphGroup->childItems();
    for (auto *item : children) {
        graphGroup->removeFromGroup(item);
        delete item;
    }
    
    children = pathGroup->childItems();
    for (auto *item : children) {
        pathGroup->removeFromGroup(item);
        delete item;
    }
}


