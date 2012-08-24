#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "glitem.h"
#include <QGLWidget>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    scene = new QScene(this);
    scene->setItemIndexMethod(QGraphicsScene::NoIndex);
    scene->setBackgroundBrush(Qt::white);
    scene->setStickyFocus(false);

    view = new QBox2DView(ui->centralWidget);
    view->setScene(scene);
    view->setViewport(new QGLWidget());
    connect(ui->actionZoomIn, SIGNAL(triggered()), view, SLOT(zoomIn()));
    connect(ui->actionZoomOut, SIGNAL(triggered()), view, SLOT(zoomOut()));
    ui->horizontalLayout->addWidget(view);
    glscene = new GLScene(this);
    ui->horizontalLayout->addWidget(glscene);

    createWorld();

    timer = new QTimer(this);
    connect(view,SIGNAL(mouseLeftButtonPressed(QPointF)),world,SLOT(grabItem(QPointF)));
    connect(view,SIGNAL(mouseRightButtonPressed(QPointF)),world,SLOT(createBox(QPointF)));
    connect(view,SIGNAL(mouseLeftButtonReleased()),world,SLOT(dropItem()));
    connect(view,SIGNAL(mouseMoved(QPointF)),world,SLOT(moveItem(QPointF)));
    connect(view,SIGNAL(keyPressed(int)),world,SLOT(handleKeyPressed(int)));
    connect(view,SIGNAL(keyReleased(int)),world,SLOT(handleKeyReleased(int)));
    connect(timer,SIGNAL(timeout()),this,SLOT(update()));

    timer->start(1000/60);

}

void MainWindow::createWorld(){
    //world = new ExampleWorld();
    world = new TestWorld();
    //world = new ArcanoidWorld();
    connect(world,SIGNAL(itemDestroyed(QGraphicsItem*)),scene,SLOT(removeItem(QGraphicsItem*)));
    connect(world,SIGNAL(itemCreated(QGraphicsItem*)),scene,SLOT(addItem(QGraphicsItem*)));

    world->setSettings(1.0f / 60.0f, 10, 10);
    world->create();
    GLItem* myitem = new GLItem();
    myitem->setPos(1,1);
    myitem->setRotation(4);
    myitem->setBodyType(b2_dynamicBody);
    myitem->createBody(world->_world);
    glscene->addItem(myitem);


}

void MainWindow::update(){
    world->step();
    scene->advance();
    glscene->updateGL();
}

MainWindow::~MainWindow(){
    delete timer;
    delete world;
    delete view;
    delete scene;
    delete ui;
}
