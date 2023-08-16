#ifndef MYGRIDWIDGET_H
#define MYGRIDWIDGET_H

#include <QObject>
#include <QWidget>
#include <QMouseEvent>

class MyGridWidget: public QWidget
{
public:
    MyGridWidget();
    explicit MyGridWidget(QWidget* parent = nullptr);
    ~MyGridWidget();
protected:
    void paintEvent(QPaintEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;
private:
    int* pCells;
    int cols;
    int rows;

    void initCells();
    void destroyCells();
    double calculateCellWidth();
    double calculateCellHeight();
};

#endif // MYGRIDWIDGET_H
