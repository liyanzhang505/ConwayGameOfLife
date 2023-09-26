#ifndef MYGRIDWIDGET_H
#define MYGRIDWIDGET_H

#include <QObject>
#include <QWidget>
#include <QMouseEvent>
#include <QTimer>
#include "conwaygame.h"

class MyGridWidget: public QWidget
{
    Q_OBJECT
public:
    MyGridWidget();
    explicit MyGridWidget(QWidget* parent = nullptr);
    ~MyGridWidget();
    void startEvolve();
    void stopTimer();
    void updateInterval(int value);
    void nextStep();
    void setProbabilityOfLive(qreal pLive);
    void randomInitGrid();
    void clearDisplay();
    void changeGridSize(int size);
protected:
    void paintEvent(QPaintEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;
private slots:
    void UpdateCellStates();
signals:
    void generationChanged(int generations);
private:
    QTimer* timer;
    int* pCells;
    int cols;
    int rows;
    int expandCount;
    int generations;
    GameBase* game;
    void autoExpandGrid();
    void initCells();
    void destroyCells();
    double calculateCellWidth();
    double calculateCellHeight();
    void initTimer();
    void initBackgroud();
    void initGame();
    qreal probabilityOfLive;
    void destory();
};

#endif // MYGRIDWIDGET_H
