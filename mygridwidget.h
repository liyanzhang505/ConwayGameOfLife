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
    void changeGridSize(int index);
    void changeRow(int value);
    void changeCollum(int value);
    void changeGame(int index);
    void changeGame(QString ruleStr);
    void changeRecordState(int state);
    void changeAutoFitState(int state);
    void setMaxGenerations(int generations);
    void SaveFile();
    void OpenFile();
    void setSyncRate(qreal rate);
    void setIsFullyAsync(bool flag);
    void runTest1();
    void runTestP0AndSyncRateAndDensity();
    void runTest2();
    void runTestLowerP0();
    void runTest1024();
protected:
    void paintEvent(QPaintEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;
private slots:
    void UpdateCellStates();
signals:
    void generationChanged(int value);
    void densityChanged(qreal value);
    void activityChanged(qreal value);
    void gridSizeChanged(int rows, int cols);
    void gameRuleChanged(QString ruleStr);
    void showDebug(QString info);
private:
    QTimer* timer;
    int* pCells;
    int* pCellsPrev;
    int cols;
    int rows;
    int expandCount;
    int generations;
    qreal density;
    qreal activity;
    BirthSurviveRule* bsRule;
    GameBase* game;
    QFile* file;
    QString gameName;
    qreal probabilityOfLive;
    bool isFullyAsync;
    qreal syncRate;
    int maxGenerations;
    bool enableRecordStatistics;
    bool enableAutoFit;
    void autoExpandGrid();
    void initCells();
    void destroyCells();
    double calculateCellWidth();
    double calculateCellHeight();
    void initTimer();
    void initBackgroud();
    void initGame();
    void initRecordStatisticsFile();
    void destory();
    void recordGameData(qreal density, qreal activity);
    QString createGameNameByBSRule(const std::vector<int> &birthRules, const std::vector<int> &survivalRules);
    qreal calculateDensity();
    qreal calculateActivity(int *pCellsPrev);
};

#endif // MYGRIDWIDGET_H
