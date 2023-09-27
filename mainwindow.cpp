#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "mygridwidget.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow), grid(new MyGridWidget(this)),
    interval(200), probabilityOfLive(0.3)
{
    ui->setupUi(this);
    ui->gridLayout->addWidget(grid);
//    ui->widgetLayout->setStretchFactor(ui->gridLayout, 8);
//    ui->widgetLayout->setStretchFactor(ui->pannelLayout, 2);
//    ui->pannelLayout->setStretchFactor(ui->controllPannel, 1);
//    ui->pannelLayout->setStretchFactor(ui->recordPannel, 1);
//    ui->pannelLayout->setStretchFactor(ui->otherPannel, 4);

    ui->timerSlider->setRange(0, 2000);
    ui->timerSlider->setValue(interval);
    ui->timerLabel->setText(QString::number(interval) + "ms");
    connect(grid,SIGNAL(generationChanged(int)), this, SLOT(onGenerationChanged(int)));


    ui->pSlider->setRange(0, 10);
    ui->pSlider->setValue(probabilityOfLive * 10);
    ui->pDisplay->setText(QString::number(probabilityOfLive));
    grid->setProbabilityOfLive(probabilityOfLive);

}


MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_autoEvolveBtn_clicked()
{
    grid->startEvolve();
}


void MainWindow::on_pauseBtn_clicked()
{
    grid->stopTimer();
}


void MainWindow::on_nextStepBtn_clicked()
{
    grid->nextStep();
}


void MainWindow::on_timerSlider_valueChanged(int value)
{
    interval = value;
    ui->timerLabel->setText(QString::number(interval) + "ms");
    grid->updateInterval(interval);
}

void MainWindow::onGenerationChanged(int value)
{
    ui->generationsLabel->setText(QString::number(value));
}


void MainWindow::on_pSlider_valueChanged(int value)
{
    probabilityOfLive = qreal(value) / 10;
    ui->pDisplay->setText(QString::number(probabilityOfLive));
    grid->setProbabilityOfLive(probabilityOfLive);
}


void MainWindow::on_RandomInit_clicked()
{
    grid->randomInitGrid();
}


void MainWindow::on_ClearBtn_clicked()
{
    grid->clearDisplay();
}


void MainWindow::on_gridSizeBox_currentIndexChanged(int index)
{
    qDebug() << "Selected item:" << ui->gridSizeBox->itemText(index) << "index:" << index;
    grid->changeGridSize(index);
}


void MainWindow::on_gameBox_currentIndexChanged(int index)
{
    grid->changeGame(index);
}

