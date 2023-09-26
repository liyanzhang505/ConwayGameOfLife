#include "mygridwidget.h"
#include <QPainter>
#include <iostream>
#include <QPalette>
#include "BirthSurviveRule.h"
#include "GridSizeOps.h"

MyGridWidget::MyGridWidget(QWidget* parent) : QWidget(parent), cols(256), rows(256)
{
    initGame();
    initBackgroud();
    initCells();
    initTimer();
}


MyGridWidget::~MyGridWidget()
{
    destory();
}

void MyGridWidget::destory()
{
    delete game;
    delete timer;
    destroyCells();
}

void MyGridWidget::initBackgroud()
{
    QPalette pal = QPalette();
    pal.setColor(QPalette::Window, Qt::white);
    this->setAutoFillBackground(true);
    this->setPalette(pal);
    this->show();
}


void MyGridWidget::initGame()
{
    expandCount = 0;
    generations = 0;
    BirthSurviveRule bsRule = BirthSurviveRule({3}, {2, 3});
    game = new ConwayGame(rows, cols);
}

void MyGridWidget::clearDisplay()
{
    timer->stop();
    std::memset(pCells, 0, rows * cols * sizeof(int));
    generations = 0;
    emit generationChanged(generations);
    update();
}

void MyGridWidget::changeGridSize(int size)
{
    timer->stop();
    delete game;
    destroyCells();

    qDebug() << "size:" << size;
    if (Grid256 == size) {
        rows = 256;
        cols = 256;
    } else if (Grid20 == size) {
        rows = 20;
        cols = 20;
    }
    generations = 0;
    expandCount = 0;
    emit generationChanged(generations);
    initGame();
    initCells();
    update();
}


void MyGridWidget::startEvolve()
{
    timer->start();
    std::cout << "Start Evolve..." << std::endl;
}

void MyGridWidget::initTimer()
{
    timer = new QTimer(this);
    timer->setInterval(100);
    connect(timer, SIGNAL(timeout()), this, SLOT(UpdateCellStates()));
}

void MyGridWidget::stopTimer()
{
    timer->stop();
}

void MyGridWidget::updateInterval(int value)
{
    timer->setInterval(value);
}

void MyGridWidget::nextStep()
{
    timer->stop();
    UpdateCellStates();
}

void MyGridWidget::setProbabilityOfLive(qreal probabilityOfLive)
{
    this->probabilityOfLive = probabilityOfLive;
}

void MyGridWidget::randomInitGrid()
{
    clearDisplay();
    srand(time(0));
    for(int i = 0; i < rows; ++i) {
        for(int j = 0; j < cols; ++j) {
            if((rand() / double(RAND_MAX)) <= probabilityOfLive) {
                pCells[i * cols + j] = 1;
            } else {
                 pCells[i * cols + j] = 0;
            }
        }
    }
    update();
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

    if (rows != 256 && comeBound > 0) {
        expandCount += 1;
        autoExpandGrid();
    }

    std::cout << "come bound:" << comeBound << std::endl;
    std::cout << "here1_UpdateCellStates" << std::endl;
    game->evolve(pCells);
    generations += 1;
    emit generationChanged(generations);
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
    qreal lineWidth = 0;
    qreal alpha = 0;

    if (rows != 256) {
        lineWidth = std::max(0.0, 1 - expandCount* 0.2);
        alpha = std::max(0.0, 0.4 - expandCount * 0.1);
        std::cout << "alpha: " << alpha << "lineWidth: " << lineWidth << std::endl;
    }

    // Line setting, set line to back with width of 1.
    QColor penColor = QColor(Qt::black);
    penColor.setAlphaF(alpha);
    QPen pen(penColor);
    std::cout << "alpha: " << alpha << "lineWidth: " << lineWidth << std::endl;
    pen.setWidthF(lineWidth);
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
                QRect cellField(x + lineWidth, y + lineWidth, calculateCellWidth() - lineWidth, calculateCellHeight() - lineWidth);
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
