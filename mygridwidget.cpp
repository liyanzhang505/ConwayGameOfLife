#include "mygridwidget.h"
#include <QPainter>
#include <iostream>
#include <QPalette>
#include "BirthSurviveRule.h"
#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <QFileInfo>
#include <QDate>
#include <QFileDialog>
#include <QMessageBox>
#include "CommonDef.h"
#include "rletools.h"


MyGridWidget::MyGridWidget(QWidget* parent) : QWidget(parent), cols(GRID_SIZE_256), rows(GRID_SIZE_256),
    probabilityOfLive(0), syncRate(1.0), isFullyAsync(false)
{
    initRecordStatisticsFile();
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
    delete bsRule;
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

void MyGridWidget::recordGameData(const QString& gameName,
                                  int gridSize,
                                  qreal p0,
                                  int evolutionCount,
                                  qreal survivePercentage) {
    if (file->open(QIODevice::WriteOnly | QIODevice::Append)) {
        QTextStream output(file);
        output << gameName << ","
               << gridSize << ","
               << p0 << ","
               << evolutionCount << ","
               << survivePercentage << "\n";

        file->close();
    } else {
        qWarning("Can not open record file....");
    }
}


void MyGridWidget::initRecordStatisticsFile()
{
    QString currentDate = QDate::currentDate().toString("yyyy_MM_dd");
    QString fileName = currentDate + "_statistics.csv";
    file = new QFile(fileName);
    QFileInfo fileInfo(*file);
    QString absolutePath = fileInfo.absoluteFilePath();
    if (fileInfo.size() == 0) {
        QStringList headers = {"GameName", "gridsize", "p0", "time", "p"};
        qDebug() << "Creating new statistics to file: " + absolutePath;
        if (file->open(QIODevice::WriteOnly)) {
            QTextStream output(file);
            output << headers.join(",") << "\n";
            file->close();
        } else {
            qWarning("Can not open record file....");
        }

    } else {
        qDebug() << "Output statistics to file: " + absolutePath;
    }
}

void MyGridWidget::setMaxGenerations(int generations)
{
    maxGenerations = generations;
}

void MyGridWidget::SaveFile()
{
    QString fileName =  QFileDialog::getSaveFileName(this,"打开文件","","All(*.*);;Text files (*.txt, *.rle)");
    if(fileName.isNull())
    {
        QMessageBox::information(this,"Warning","Havn't input the filename.");
        return;
    }

    qDebug() << "Save file to :" << fileName;

    RLETools::saveToRLE(fileName, pCells, rows, cols, game->getBirthSetting(), game->getSurviveSetting());
}

void MyGridWidget::OpenFile()
{
    int * tmpPcells = NULL;
    int tmpRows = 0;
    int tmpCols = 0;
    std::vector<int> birthSetting;
    std::vector<int> surviveSetting;
//    QString fFileName = "/Users/yanzhangli/Qtproject/rle/glider1.rle";


    QString fileName = QFileDialog::getOpenFileName(this, "Open existing rle file","","RLE files(*.rle)");
    if(fileName.isNull())
    {
        QMessageBox::information(this,"Warning","Havn't select a file.");
        return;
    }

    if (RLETools::loadFromRLE(fileName,tmpPcells, tmpRows, tmpCols, birthSetting, surviveSetting) != true) {
        QMessageBox::information(this,"Error","Load rle file error!");
        return;
    }

    timer->stop();
    delete game;
    destroyCells();

    pCells = tmpPcells;
    rows = tmpRows;
    cols = tmpCols;
    generations = 0;
    expandCount = 0;
    qDebug() << "load game rows:" << rows << "cols:" << cols;
    bsRule = new BirthSurviveRule(birthSetting, surviveSetting);
    game = new ConwayGame(rows, cols, bsRule);
    gameName = createGameNameByBSRule(birthSetting, surviveSetting);

    emit generationChanged(generations);
    emit gridSizeChanged(rows, cols);
    emit gameRuleChanged(gameName);
    update();
}

qreal MyGridWidget::calculateSurviveRate()
{
    int sum = 0;
    for(int i = 0; i < rows; ++i) {
        for(int j = 0; j < cols; ++j) {
            if(pCells[i * cols + j] == 1) {
                sum++;
            }
        }
    }

    return qreal(sum) / (rows * cols);
}


void MyGridWidget::initGame()
{
    expandCount = 0;
    generations = 0;
    density = 0.0;
    activity = 0.0;
    if (bsRule == NULL) {
        bsRule = new BirthSurviveRule({3}, {2, 3});
        game = new ConwayGame(rows, cols, bsRule);
        gameName =GAME_NAME_CONWAYGAME;
        emit gameRuleChanged(gameName);
    } else {
        game = new ConwayGame(rows, cols, bsRule);
    }
}

void MyGridWidget::clearDisplay()
{
    timer->stop();
    std::memset(pCells, 0, rows * cols * sizeof(int));
    generations = 0;
    density = 0;
    activity = 0;
    emit generationChanged(generations);
    emit densityChanged(density);
    emit activityChanged(activity);
    update();
}

void MyGridWidget::changeGame(QString ruleStr) {
    qDebug()<<"change game rule : " << ruleStr;
    delete bsRule;
    delete game;

    std::vector<int> birthSetting;
    std::vector<int> surviveSetting;
    RLETools::parseRule(ruleStr, birthSetting, surviveSetting);

    bsRule = new BirthSurviveRule(birthSetting, surviveSetting);
    game = new ConwayGame(rows, cols, bsRule);

    gameName = ruleStr;
    qDebug() << "Change to game: " << gameName;
    emit gameRuleChanged(gameName);
    clearDisplay();
}


void MyGridWidget::changeGame(int index) {
    qDebug()<<"change game index : " << index;
    delete bsRule;
    delete game;
    switch(index) {
        case GAME_INDEX_CONWAYGAME:
            bsRule = new BirthSurviveRule({3}, {2, 3});
            game = new ConwayGame(rows, cols, bsRule);
            break;
        case GAME_INDEX_HIGHLIFE:
            bsRule = new BirthSurviveRule({3, 6}, {2, 3});
            game = new ConwayGame(rows, cols, bsRule);
            break;
        case GAME_INDEX_PSEUDOLIFE:
            bsRule = new BirthSurviveRule({3, 5, 7}, {2, 3, 8});
            game = new ConwayGame(rows, cols, bsRule);
            break;
        case GAME_INDEX_2X2:
            bsRule = new BirthSurviveRule({3, 6}, {1, 2, 5});
            game = new ConwayGame(rows, cols, bsRule);
            break;
        case GAME_INDEX_MOVE:
            bsRule = new BirthSurviveRule({3, 6, 8}, {2, 4, 5});
            game = new ConwayGame(rows, cols, bsRule);
            break;
        case GAME_INDEX_B2S2:
            bsRule = new BirthSurviveRule({2}, {2});
            game = new ConwayGame(rows, cols, bsRule);
            break;
        default:
            bsRule = new BirthSurviveRule({3}, {2, 3});
            game = new ConwayGame(rows, cols, bsRule);
    }

    gameName = createGameNameByBSRule(bsRule->getBirthSetting(), bsRule->getSurviveSetting());
    qDebug() << "Change to game: " << gameName;
    emit gameRuleChanged(gameName);
    clearDisplay();
}

QString MyGridWidget::createGameNameByBSRule(const std::vector<int> &birthRules, const std::vector<int> &survivalRules) {
    QString name = "B";
    for (int rule : bsRule->getBirthSetting()) {
        name += QString::number(rule);
    }

    name += "/S";

    for (int rule : bsRule->getSurviveSetting()) {
        name += QString::number(rule);
    }

    return name;
}

void MyGridWidget::setSyncRate(qreal rate)
{
    syncRate = rate;
}

void MyGridWidget::setIsFullyAsync(bool flag)
{
    isFullyAsync = flag;
}

void MyGridWidget::changeRecordState(int state)
{
    if (state == Qt::Checked) {
        enableRecordStatistics = true;
    } else {
        enableRecordStatistics = false;
    }
}

void MyGridWidget::changeAutoFitState(int state)
{
    if (state == Qt::Checked) {
        enableAutoFit = true;
    } else {
        enableAutoFit = false;
    }
}

void MyGridWidget::changeGridSize(int index)
{
    timer->stop();
    delete game;
    destroyCells();

    qDebug() << "grid index:" << index;
    if (GRID_INDEX_256 == index) {
        rows = GRID_SIZE_256;
        cols = GRID_SIZE_256;
    } else if (GRID_INDEX_20 == index) {
        rows = GRID_SIZE_20;
        cols = GRID_SIZE_20;
    }
    generations = 0;
    expandCount = 0;
    emit generationChanged(generations);
    emit gridSizeChanged(rows, cols);

    initGame();
    initCells();
    update();
}

void MyGridWidget::changeRow(int value)
{
    timer->stop();
    delete game;
    destroyCells();

    rows = value;
    generations = 0;
    expandCount = 0;
    emit generationChanged(generations);
    emit gridSizeChanged(rows, cols);

    initGame();
    initCells();
    update();
}

void MyGridWidget::changeCollum(int value)
{
    timer->stop();
    delete game;
    destroyCells();

    cols = value;
    generations = 0;
    expandCount = 0;
    emit generationChanged(generations);
    emit gridSizeChanged(rows, cols);

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
    qDebug() << "probabilityOfLive" << probabilityOfLive;
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
    if (enableRecordStatistics) {
        qreal surviveRate = calculateSurviveRate();
        qDebug() << "init surviveRate: " << surviveRate;
        recordGameData(gameName, rows, probabilityOfLive, generations, surviveRate);
    }
}



void MyGridWidget::UpdateCellStates()
{
    if (enableAutoFit && rows <= 256) {
        int comeBound = 0;
        for (int row = 0; row < rows; row++) {
            comeBound += pCells[row * cols];
            comeBound += pCells[(row + 1) * cols -1];
        }
        for (int col = 1; col < cols - 1; col++) {
            comeBound += pCells[col];
            comeBound += pCells[(rows -1) * cols + col];
        }

        if ((rows < GRID_SIZE_AUTO_FIT_LIMIT) && comeBound > 0) {
            expandCount += 1;
            autoExpandGrid();
        }
    }

    int* pCellsPrev = new int[rows * cols];
    memcpy(pCellsPrev, pCells, rows * cols * sizeof(int));

    if (isFullyAsync) {
        game->fullyAsyncEvolve(pCells);
    } else {
        game->partialSyncEvolve(pCells, syncRate);
    }

    generations += 1;
    density = calculateDensity();
    activity = calculateActivity(pCellsPrev);

    emit densityChanged(density);
    emit activityChanged(activity);
    emit generationChanged(generations);
    delete[] pCellsPrev;

    update();

    if (enableRecordStatistics) {
        if (generations >= maxGenerations) {
            timer->stop();
        }
        qreal surviveRate = calculateSurviveRate();
        qDebug() << "surviveRate: " << surviveRate;
        recordGameData(gameName, rows, probabilityOfLive, generations, surviveRate);
    }

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
    qreal lineWidth = 0.0;
    qreal alpha = 0.0;

    if (rows > 160) {
        qreal lineWidth = 0.0;
        qreal alpha = 0.0;
    } else if (rows > 80 && rows <= 160) {
        alpha = 0.1;
        lineWidth = 0.4;
    } else if (rows > 40 && rows <= 80) {
        alpha = 0.2;
        lineWidth = 0.6;
    } else if (rows > 20 && rows <= 40) {
        alpha = 0.3;
        lineWidth = 0.8;
    } else {
        alpha = 0.4;
        lineWidth = 1.0;
    }



    std::cout << "row:" << rows << " alpha: " << alpha << " lineWidth: " << lineWidth << std::endl;

    // Line setting, set line to back with width of 1.
    QColor penColor = QColor(Qt::black);
    penColor.setAlphaF(alpha);
    QPen pen(penColor);

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


qreal MyGridWidget::calculateDensity() {
    int totalCells = rows * cols;
    int survivalCells = 0;

    for(int i = 0; i < totalCells; ++i) {
        if(pCells[i] == 1) {
            survivalCells++;
        }
    }

    return static_cast<double>(survivalCells) / totalCells;
}

qreal MyGridWidget::calculateActivity(int* pCellsPrev) {
    int totalCells = rows * cols;
    int changedCells = 0;

    for(int i = 0; i < totalCells; ++i) {
        if(pCells[i] != pCellsPrev[i]) {
            changedCells++;
        }
    }

    return static_cast<double>(changedCells) / totalCells;
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

    emit gridSizeChanged(rows, cols);
}
