
#include <QtWidgets/QApplication>
#include <QtWidgets/QWidget>
#include <QtWidgets/QGraphicsView>
#include <QtWidgets/QGraphicsScene>
#include <QtWidgets/QGraphicsEllipseItem>
#include <QtWidgets/QGraphicsLineItem>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QMessageBox>
#include <QtWidgets/QGraphicsItemGroup>
#include <Qdir>
#include <QCoreApplication>



#include "Graph.h"
#include "AStar.h"


#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <queue>
#include <functional>
#include <math.h>






void showHelpMessage() {
    QMessageBox::information(nullptr,
                            "Help",
                            "This is the help message!\n"
                            "Here you can explain what your application does.");
}


int main(int argc, char** argv) {
    srand(time(0));
    
    Graph g;
    
    
    
    
  
    
    
    
    
    
    
    QApplication app(argc, argv);
    
//    QString filePath = QDir::current().absoluteFilePath("input.txt");

    
    QString exePath = QCoreApplication::applicationDirPath();
    QString localPath = exePath + "/../../../input.txt";
    
    
    QMessageBox::information(nullptr,
                            "Путь к файлу",
                            "Программа ищет файл по пути:\n" + localPath);
    
    
    
    
    QWidget wgt;
    QHBoxLayout* mainLayout = new QHBoxLayout;
    
    
    QGraphicsScene* scene = new QGraphicsScene(QRectF(-100, -100, 640, 480));

    
    QGraphicsItemGroup* qgraph = new QGraphicsItemGroup;
    scene->addItem(qgraph);
    
    QGraphicsItemGroup* path = new QGraphicsItemGroup;
    scene->addItem(path);
    
 
    
    
    
    
    
    
    
    
    QGraphicsView* pview = new QGraphicsView(scene);
    mainLayout->addWidget(pview);
    QVBoxLayout* buttons = new QVBoxLayout;
    QPushButton* btn1 = new QPushButton("READ input.txt");
    QPushButton* btn2 = new QPushButton("SHOW/HIDE");
    
    
    
    
    buttons->addWidget(btn1);
    buttons->addWidget(btn2);

    
    QObject::connect(btn1, &QPushButton::clicked, [&g, qgraph, path, scene, localPath](){
        int res = g.read_from_file(localPath.toStdString());
        if (res != 0) {
            std::string s = std::string("BAD FILE, code ") + std::to_string(res);
            QMessageBox::information(nullptr,
                                    "Error",
                                    s.c_str());
        } else {
            auto children = qgraph->childItems();
                    for (QGraphicsItem* item : children) {
                        qgraph->removeFromGroup(item);
                        delete item;
                    }
                    
                    children = path->childItems();
                    for (QGraphicsItem* item : children) {
                        path->removeFromGroup(item);
                        delete item;
                    }
            g.init_info();
            g.process(100);

            
            int numNodes = g.ptrs.size();
            std::vector<std::vector<bool>> connections(numNodes, std::vector<bool>(numNodes, false));
            
            
            for (int v = 0; v < numNodes; v++) {
                
                Graph::info::Point pv = g.infos[v].p;

                QGraphicsEllipseItem *point = new QGraphicsEllipseItem(0, 0, 10, 10);
                point->setPos((int)pv.x, (int)pv.y);
                point->setBrush(Qt::black);
                qgraph->addToGroup(point);
                
                int bound = ((v == numNodes - 1) ? (g.edges.size()) : (g.ptrs[v + 1]));
                for (int edge_cnt = g.ptrs[v]; edge_cnt < bound; edge_cnt++) {
                    int u = g.edges[edge_cnt].first;
                    if (connections[u][v]) {
                        continue;
                    }
                    connections[u][v] = connections[v][u] = 1;
                    Graph::info::Point pu = g.infos[u].p;
                    
                    QGraphicsLineItem *line = new QGraphicsLineItem((int)pv.x + 5, (int)pv.y + 5, (int)pu.x + 5, (int)pu.y + 5);
                        line->setPen(QPen(Qt::black, 2));
                        qgraph->addToGroup(line);
                    

                }
            }
            
            
            
            
            
            
            
            
            
            
            
            
            
            
            
            std::function<double(double, double)> return_zero = [](double a, double b) {return 0;};
            auto v = AStar::findPath(0, g.ptrs.size() - 1, return_zero, g);
            if (!v.empty()) {
                for (int i = 0; i < v.size() - 1; i++) {
                    Graph::info::Point p = g.infos[v[i]].p;
                    if (i != 0) {
                        Graph::info::Point pred = g.infos[v[i - 1]].p;
                        QGraphicsLineItem *line = new QGraphicsLineItem((int)pred.x + 5, (int)pred.y + 5, (int)p.x + 5, (int)p.y + 5);
                            line->setPen(QPen(Qt::red, 2));
                            path->addToGroup(line);
                    }
                    QGraphicsEllipseItem *point = new QGraphicsEllipseItem(0, 0, 10, 10);
                    point->setPos((int)p.x, (int)p.y);
                    point->setBrush(Qt::red);
                    path->addToGroup(point);
                }
            } else {
                QMessageBox::information(nullptr,
                                            "Path Finding",
                                            "No path found between nodes!");
            }
            
            
            

        }
    });
    QObject::connect(btn2, &QPushButton::clicked, [path]() {
           path->setVisible(!path->isVisible());
       });
    
    
    
    
    mainLayout->addLayout(buttons);
    
    wgt.setLayout(mainLayout);
    wgt.show();
    return app.exec();
}



//
//class MyView: public QGraphicsView {
//    Q_OBJECT
//public:
//    MyView(QGraphicsScene* pScene, QWidget* pwgt = nullptr) : QGraphicsView(pScene, pwgt) {}
//public slots:
//    void slotZoomIn() {
//        scale(1.1, 1.1);
//    }
//    void slotZoomOut(){
//        scale(1 / 1.1, 1 / 1.1);
//    }
//    void slotRotateLeft() {
//        rotate(-5);
//    }
//    void slotRotateRight() {
//        rotate(5);
//    }
//};
//
//class SimpleItem : public QGraphicsItem {
//private:
//    enum {nPenWidth = 3};
//    
//public:
//    virtual QRectF boundingRect() const {
//        QPointF ptPosition(-10 - nPenWidth, -10 -nPenWidth);
//        QSizeF size(20 + nPenWidth*2, 20 + nPenWidth*2);
//        return QRectF(ptPosition, size);
//    }
//    
//    
//    virtual void paint(QPainter* ppainter, const QStyleOptionGraphicsItem*, QWidget*) {
//        ppainter->save();
//        ppainter->setPen(QPen(Qt::blue, nPenWidth));
//        ppainter->drawEllipse(-10, -10, 20, 20);
//        ppainter->restore();
//    }
//    
//    virtual void mousePressEvent(QGraphicsSceneMouseEvent* pe) {
//        QApplication::setOverrideCursor(Qt::PointingHandCursor);
//        QGraphicsItem::mousePressEvent(pe);
//    }
//    virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent* pe) {
//        QApplication::restoreOverrideCursor();
//        QGraphicsItem::mouseReleaseEvent(pe);
//    }
//    
//};
//
//
//int main(int argc, char** argv) {
//    QApplication app(argc, argv);
////    QGraphicsScene scene(QRectF(-100, -100, 300, 300));
////    QGraphicsView view(&scene);
//    
////    QGraphicsRectItem* pRectItem = scene.addRect(QRectF(-30, -30, 120, 80), QPen(Qt::black), QBrush(Qt::green));
////    pRectItem->setFlags(QGraphicsItem::ItemIsMovable);
////    
////    QGraphicsPixmapItem* pPixmapItem = scene.addPixmap(QPixmap("./banner.jpg"));
////    pPixmapItem->setFlags(QGraphicsItem::ItemIsMovable);
////    
////    QGraphicsTextItem* pText = scene.addText("I LIKE TO MOVE IT, MOVE IT");
////    pText->setFlags(QGraphicsItem::ItemIsMovable);
//    
////    QGraphicsLineItem* pLine = scene.addLine(QLineF(-10, -10, -80, -80), QPen(Qt::red, 2));
////    pLine->setFlags(QGraphicsItem::ItemIsMovable);
////    QGraphicsTextItem* pTex = scene.addText("Child");
////    pTex->setParentItem(pLine);
////    
////    QGraphicsTextItem* pText1 = scene.addText("Shear");
////    pText1->setTransform(QTransform().shear(-0.5, 0.0), true);
////
////    
//    
//    QGraphicsScene scene(QRectF(-100, -100, 640, 480));
//    
//    MyView* pview = new MyView(&scene);
//    QPushButton* pcmdZoomIn = new QPushButton("&Zoom in");
//    
//    
//    pview->setRenderHint(QPainter::Antialiasing, true);
//    
//    SimpleItem* pSimpleItem = new SimpleItem;
//    scene.addItem(pSimpleItem);
//    pSimpleItem->setPos(0, 0);
//    pSimpleItem->setFlags(QGraphicsItem::ItemIsMovable);
//    
//    QObject::connect(pcmdZoomIn, SIGNAL(clicked()), pview, SLOT(slotZoomIn));
//    
//    
//    QVBoxLayout* pvbxLayout = new QVBoxLayout;
//    pvbxLayout->addWidget(pview);
//    pvbxLayout->addWidget(pcmdZoomIn);
//    
//    wgt.setLayout(pvbxLayout);
//    
//    
//    
//    wgt.show();
//    return app.exec();
//}


//
//void MainWindow::loadGraph()
//{
//    // Очистка предыдущего графа
//    clearScene();
//    
//    // Загрузка файла
//
//    if (filePath.isEmpty()) return;
//    
//    // Загрузка и обработка графа
//    if (graph.readFromFile(filePath.toStdString()) != 0) {
//        QMessageBox::critical(this, "Ошибка", "Не удалось загрузить файл!");
//        return;
//    }
//    
//    graph.initInfo();
//    graph.process(100);
//    
//    // Визуализация графа
//    drawGraph();
//}
//
//void MainWindow::drawGraph()
//{
//    int numNodes = graph.ptrs.size();
//    std::vector<std::vector<bool>> connections(numNodes,
//                                             std::vector<bool>(numNodes, false));
//    
//    // Рисуем узлы и связи
//    for (int v = 0; v < numNodes; v++) {
//        auto pv = graph.infos[v].p;
//        
//        // Узел графа
//        auto *node = new QGraphicsEllipseItem(0, 0, 20, 20);
//        node->setPos(pv.x - 10, pv.y - 10);
//        node->setBrush(Qt::blue);
//        graphGroup->addToGroup(node);
//        
//        // Подпись узла
//        auto *label = new QGraphicsSimpleTextItem(QString::number(v));
//        label->setPos(pv.x, pv.y);
//        graphGroup->addToGroup(label);
//        
//        // Связи
//        int bound = (v == numNodes - 1) ? graph.edges.size() : graph.ptrs[v + 1];
//        for (int e = graph.ptrs[v]; e < bound; e++) {
//            int u = graph.edges[e].first;
//            if (connections[u][v]) continue;
//            connections[u][v] = connections[v][u] = true;
//            
//            auto pu = graph.infos[u].p;
//            auto *edge = new QGraphicsLineItem(pv.x, pv.y, pu.x, pu.y);
//            edge->setPen(QPen(Qt::black, 2));
//            graphGroup->addToGroup(edge);
//        }
//    }
//}
//
//void MainWindow::showPath()
//{
//    // Очистка предыдущего пути
//    pathGroup->setVisible(false);
//    auto children = pathGroup->childItems();
//    for (auto *item : children) {
//        pathGroup->removeFromGroup(item);
//        delete item;
//    }
//    
//    // Поиск пути
//    auto path = AStar::findPath(0, graph.ptrs.size() - 1,
//                              [](double, double){ return 0; },
//                              graph);
//    
//    if (path.empty()) {
//        QMessageBox::warning(this, "Путь", "Путь не найден!");
//        return;
//    }
//    
//    // Визуализация пути
//    for (size_t i = 0; i < path.size() - 2; i++) {
//        int node = path[i];
//        auto p = graph.infos[node].p;
//        
//        // Узел пути
//        auto *pathNode = new QGraphicsEllipseItem(0, 0, 20, 20);
//        pathNode->setPos(p.x - 10, p.y - 10);
//        pathNode->setBrush(Qt::red);
//        pathGroup->addToGroup(pathNode);
//        
//        // Линия пути
//        if (i > 0) {
//            int prevNode = path[i-1];
//            auto pPrev = graph.infos[prevNode].p;
//            auto *pathEdge = new QGraphicsLineItem(pPrev.x, pPrev.y, p.x, p.y);
//            pathEdge->setPen(QPen(Qt::red, 3));
//            pathGroup->addToGroup(pathEdge);
//        }
//    }
//    
//    pathGroup->setVisible(true);
//    QMessageBox::information(this, "Путь",
//                            QString("Длина пути: %1").arg(path.back()));
//}
