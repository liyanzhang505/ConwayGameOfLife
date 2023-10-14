#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "mygridwidget.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow), grid(new MyGridWidget(this)),
    interval(200), probabilityOfLive(0.3)
{
    ui->setupUi(this);
    ui->gridLayout->addWidget(grid);

    grid->changeRecordState(Qt::Unchecked);
    grid->changeAutoFitState(Qt::Unchecked);
    grid->setMaxGenerations(512);
    ui->maxGenerationsEdit->setText(QString::number(512));
    QIntValidator *validator = new QIntValidator(0, 512, this);
    ui->maxGenerationsEdit->setValidator(validator);
    ui->maxGenerationsEdit->setDisabled(true);
    ui->maxGenerationsSetBtn->setDisabled(true);

    ui->timerSlider->setRange(0, 2000);
    ui->timerSlider->setValue(interval);
    ui->timerLabel->setText(QString::number(interval) + "ms");
    connect(grid,SIGNAL(generationChanged(int)), this, SLOT(onGenerationChanged(int)));
    connect(grid,SIGNAL(densityChanged(qreal)), this, SLOT(onDensityChanged(qreal)));
    connect(grid,SIGNAL(activityChanged(qreal)), this, SLOT(onActivityChanged(qreal)));


    ui->pSlider->setRange(0, 10);
    ui->pSlider->setValue(probabilityOfLive * 10);
    ui->pDisplay->setText(QString::number(probabilityOfLive));
    grid->setProbabilityOfLive(probabilityOfLive);

    qDebug() <<"window width:"  << width() << "height:" << height();



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

void MainWindow::onDensityChanged(qreal value)
{
    ui->densityLabel->setText(QString::number(value));
}

void MainWindow::onActivityChanged(qreal value)
{
    ui->activityLabel->setText(QString::number(value));
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


void MainWindow::on_EnableRecordCheckBox_stateChanged(int state)
{
    if (state == Qt::Checked) {
        ui->maxGenerationsEdit->setDisabled(false);
        ui->maxGenerationsSetBtn->setDisabled(false);
    } else {
        ui->maxGenerationsEdit->setDisabled(true);
        ui->maxGenerationsSetBtn->setDisabled(true);
    }
    grid->changeRecordState(state);
}


void MainWindow::on_saveFileBtn_clicked()
{
    grid->SaveFile();
}


void MainWindow::on_openFileBtn_clicked()
{
    grid->OpenFile();
}


void MainWindow::on_autoFitCheckBox_stateChanged(int state)
{
    grid->changeAutoFitState(state);
}

