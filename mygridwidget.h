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
    void testCSV();

    void startEvolve();
    void stopTimer();
    void updateInterval(int value);
    void nextStep();
    void setProbabilityOfLive(qreal pLive);
    void randomInitGrid();
    void clearDisplay();
    void changeGridSize(int index);
    void changeGame(int index);
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
    QFile* file;
    QString gameName;
    qreal p0;
    void autoExpandGrid();
    void initCells();
    void destroyCells();
    double calculateCellWidth();
    double calculateCellHeight();
    void initTimer();
    void initBackgroud();
    void initGame();
    void initOutputFile();
    qreal probabilityOfLive;
    void destory();
    void recordGameData(const QString &gameName, qreal p0, int evolutionCount, qreal p);
};

#endif // MYGRIDWIDGET_H
