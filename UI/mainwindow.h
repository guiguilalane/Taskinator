#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include <UI/newlist.h>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    
private slots:
    void on_actionNouveau_triggered();

    void on_actionOuvrir_triggered();

private:
    Ui::MainWindow *ui;
    NewList * newList_;
};

#endif // MAINWINDOW_H
