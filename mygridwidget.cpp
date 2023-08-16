#include "mygridwidget.h"
#include <QPainter>
#include <iostream>

MyGridWidget::MyGridWidget(QWidget* parent) : QWidget(parent), cols(20), rows(20)
{
    initCells();
    this->setStyleSheet("background-color: white;");
}

MyGridWidget::~MyGridWidget()
{
    destroyCells();
}

void MyGridWidget::initCells()
{
    pCells = new int[rows * cols];
    memset(pCells, 0, rows * cols * sizeof(int));
}

void MyGridWidget::destroyCells()
{
    delete[] pCells;
}


double MyGridWidget::calculateCellWidth()
{
    return (double)width() / cols;
}

double MyGridWidget::calculateCellHeight()
{
    return (double)height() / rows;
}


void MyGridWidget::paintEvent(QPaintEvent* event)
{

    QPainter painter(this);

    // Line setting, set line to gray with width of 1.
    QColor penColor = QColor(Qt::black);
    penColor.setAlpha(80);
    QPen pen(penColor);
    pen.setWidth(1);
    painter.setPen(pen);


    // Draw horizontal lines
    double cellWidth = calculateCellWidth();
    double cellHeight = calculateCellHeight();
    for(double k = 0; k <= width(); k += cellWidth)
    {
        painter.drawLine(k, 0, k, height());
    }

    // Draw vertical lines
    for(double k = 0; k <= height(); k += cellHeight)
    {
        painter.drawLine(0, k, width(), k);
    }

    std::cout << "rows:" << rows << "height:"<< cols;

    // Paint alive cell
    for (int row = 0; row < rows; row++)
    {
        for (int col = 0; col < cols; col++)
        {
            if (1 == pCells[row * cols + col])
            {
                double x = calculateCellWidth() * col;
                double y = calculateCellHeight() * row;
                QRect cellField(x + 1, y + 1, calculateCellWidth() - 1, calculateCellHeight() - 1);
                painter.setBrush(QBrush(Qt::black));
                painter.fillRect(cellField, painter.brush());
            }
        }
    }

}


void MyGridWidget::mousePressEvent(QMouseEvent *e)
{
    double cellWidth = calculateCellWidth();
    double cellHeight = calculateCellHeight();
    int row = floor(e->y() / cellHeight);
    int col = floor(e->x() / cellWidth);
    int last = pCells[row * cols + col];
    pCells[row * cols + col] = last == 1 ? 0 : 1;
    update();
}
