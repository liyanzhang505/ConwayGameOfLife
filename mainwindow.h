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
    void on_timerSlider_valueChanged(int value);
    void on_pSlider_valueChanged(int value);

    void on_RandomInit_clicked();


    void on_ClearBtn_clicked();

    void on_gridSizeBox_currentIndexChanged(int index);

    void on_gameBox_currentIndexChanged(int index);

public slots:
    void onGenerationChanged(int value);
private:
    Ui::MainWindow *ui;
    MyGridWidget* grid;
    int interval;
    qreal probabilityOfLive;
};
#endif
