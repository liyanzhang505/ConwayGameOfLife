#include "mygridwidget.h"
#include <QPainter>
#include <iostream>
#include <QPalette>

MyGridWidget::MyGridWidget(QWidget* parent) : QWidget(parent), cols(20), rows(20)
{
    game = new ConwayGame(rows, cols);
    initBackgroud();
    initCells();
    initTimer();
}



MyGridWidget::~MyGridWidget()
{
    destroyCells();
    delete timer;
}

void MyGridWidget::initBackgroud()
{
    QPalette pal = QPalette();
    pal.setColor(QPalette::Window, Qt::white);
    this->setAutoFillBackground(true);
    this->setPalette(pal);
    this->show();
}

void MyGridWidget::startEvolve()
{
    timer->start();
     std::cout << "Start Evolve..." << std::endl;
}

void MyGridWidget::initTimer()
{
    timer = new QTimer(this);
    timer->setInterval(500);
    connect(timer, SIGNAL(timeout()), this, SLOT(UpdateCellStates()));
}

void MyGridWidget::stopTimer()
{
    timer->stop();
}

void MyGridWidget::nextStep()
{
    timer->stop();
    UpdateCellStates();
}



void MyGridWidget::UpdateCellStates()
{
    int comeBound = 0;
    for (int row = 0; row < rows; row++) {
        comeBound += pCells[row * cols];
        comeBound += pCells[(row + 1) * cols -1];
    }
    for (int col = 1; col < cols - 1; col++) {
        comeBound += pCells[col];
        comeBound += pCells[(rows -1) * cols + col];
    }

    if (comeBound > 0) {
        autoExpandGrid();
    }

    std::cout << "come bound:" << comeBound << std::endl;
    std::cout << "here1_UpdateCellStates" << std::endl;
    game->evolve(pCells);
    update();
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

void MyGridWidget::autoExpandGrid()
{
    int rows2 = rows * 2;
    int cols2 = cols * 2;
    int* pCells2 = new int[rows2 * cols2];
    memset(pCells2, 0, rows2 * cols2 * sizeof(int));

    int startRowInx = rows2 / 4;
    int startColInx = cols2 / 4;

    for (int row = 0; row < rows; row++)
    {
        std:memcpy(pCells2 + (startRowInx + row) * cols2 + startColInx, pCells +  row* cols, cols * sizeof(int));
    }

    // todo
    int* tmp = pCells;
    pCells = pCells2;
    rows = rows2;
    cols = cols2;
    delete[] tmp;
    game->reset(rows, cols);
}
