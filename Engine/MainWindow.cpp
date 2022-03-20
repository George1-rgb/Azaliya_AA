#include "MainWindow.h"
#include "ui_MainWindow.h"
#include "widget.h"
#include <QLabel>
#include <QTimer>
#include <QResizeEvent>
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setup();
}

MainWindow::~MainWindow()
{
    delete ui;
    delete scene;

}

void MainWindow::resizeEvent(QResizeEvent *event)
{
    scene->resize(event->size().width(), event->size().height());
}

void MainWindow::setup()
{
    scene = new widget(this);
    updateWindow();
}

void MainWindow::updateWindow()
{
    scene->resize(width(), height());
    int xCenter = (width() / 2) - (scene->width() / 2);
    int yCenter = (height() / 2) - (scene->height() / 2);
    scene->move(xCenter, yCenter);
    update();
}


