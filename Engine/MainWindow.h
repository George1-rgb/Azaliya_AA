#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "Types.h"

class widget;
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
protected:
    virtual void resizeEvent(QResizeEvent *event) override;
private slots:
    void on_sphereButton_clicked();

    void on_thoreButton_clicked();

private:
    Ui::MainWindow *ui;
    widget* scene;

    void setup();
    void updateWindow();
};

#endif // MAINWINDOW_H
