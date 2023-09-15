#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QMainWindow>
#include "mygridwidget.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_autoEvolveBtn_clicked();
    void on_pauseBtn_clicked();

    void on_nextStepBtn_clicked();

private:
    Ui::MainWindow *ui;
    MyGridWidget* grid;
};
#endif
