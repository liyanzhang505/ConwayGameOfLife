#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "mygridwidget.h"
#include <QRegularExpression>
#include <QRegularExpressionValidator>
#include <QInputDialog>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow), grid(new MyGridWidget(this)),
    interval(200), probabilityOfLive(0.5)
{
    ui->setupUi(this);
    ui->gridLayout->addWidget(grid);

    grid->changeRecordState(Qt::Unchecked);
    grid->changeAutoFitState(Qt::Unchecked);
    grid->setMaxGenerations(512);
    ui->maxGenerationsEdit->setText(QString::number(512));
    QIntValidator *validator = new QIntValidator(0, 1000000000, this);
    ui->maxGenerationsEdit->setValidator(validator);

    ui->syncRateEdit->setText("1.0");
    QDoubleValidator *validator2 = new QDoubleValidator(0.000001, 1.0, 6, this);
    validator2->setNotation(QDoubleValidator::StandardNotation);
    ui->syncRateEdit->setValidator(validator2);

    ui->timerSlider->setRange(0, 2000);
    ui->timerSlider->setValue(interval);
    ui->timerLabel->setText(QString::number(interval) + "ms");

    int defaultGridSize = 50;
    ui->rowSlider->setRange(20, 2000);
    ui->collumSlider->setRange(20, 2000);
    ui->rowSlider->setValue(defaultGridSize);
    ui->collumSlider->setValue(defaultGridSize);
    ui->rowDisplay->setText("Rows: " + QString::number(defaultGridSize));
    ui->collumDisplay->setText("Collums: " + QString::number(defaultGridSize));
    grid->changeRow(defaultGridSize);
    grid->changeCollum(defaultGridSize);
    ui->currentGridLabel->setText(QString::number(defaultGridSize) + " X " + QString::number(defaultGridSize));

    ui->pSlider->setRange(0, 10);
    ui->pSlider->setValue(probabilityOfLive * 10);
    ui->pDisplay->setText(QString::number(probabilityOfLive));
    grid->setProbabilityOfLive(probabilityOfLive);

    connect(grid, SIGNAL(gridSizeChanged(int,int)), this, SLOT(onGridChanged(int,int)));
    connect(grid,SIGNAL(generationChanged(int)), this, SLOT(onGenerationChanged(int)));
    connect(grid,SIGNAL(densityChanged(qreal)), this, SLOT(onDensityChanged(qreal)));
    connect(grid,SIGNAL(activityChanged(qreal)), this, SLOT(onActivityChanged(qreal)));
    connect(grid, SIGNAL(gameRuleChanged(QString)), this, SLOT(onGameRuleChanged(QString)));
    connect(grid, SIGNAL(showDebug(QString)), this, SLOT(onAppendDebugShow(QString)));
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

void MainWindow::onGridChanged(int rows, int cols)
{
    ui->currentGridLabel->setText(QString::number(rows) + " X " + QString::number(cols));

    if (rows != ui->rowSlider->value()) {
        qDebug() << "sdfsfsfsfsfsf";
        ui->rowSlider->setValue(rows);
        ui->rowDisplay->setText("Rows: " + QString::number(rows));
    }

    if (cols != ui->collumSlider->value()) {
        ui->collumSlider->setValue(cols);
        ui->collumDisplay->setText("Collums: " + QString::number(cols));
    }
}

void MainWindow::onGameRuleChanged(QString ruleStr)
{
    ui->currentGameRuleLabel->setText(ruleStr);
}

void MainWindow::onAppendDebugShow(QString info)
{
    ui->debugBrowser->append(info);
    ui->debugBrowser->moveCursor(QTextCursor::End);
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


void MainWindow::on_gameBox_currentIndexChanged(int index)
{
    if (index < 6) {
        grid->changeGame(index);
    }
}


void MainWindow::on_EnableRecordCheckBox_stateChanged(int state)
{
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


void MainWindow::on_setSyncRateBtn_clicked()
{
    QString text = ui->syncRateEdit->text();
    qDebug() << "set Sync rate:" << text;
    bool convertSuccess;
    qreal syncRate = text.toDouble(&convertSuccess);

    if (convertSuccess && syncRate > 0 && syncRate <= 1) {
        qDebug() << "Sync Rate:" << syncRate;
        grid->setSyncRate(syncRate);
    } else {
        ui->syncRateEdit->setText("1.0");
        QMessageBox::critical(this, tr("Error"), tr("Invalid input. Please enter a valid number range from 0 to 1"));
    }
}


void MainWindow::on_fullyAsyncCheckBox_stateChanged(int state)
{
    if (state == Qt::Checked) {
        grid->setIsFullyAsync(true);
        ui->syncRateEdit->setDisabled(true);
        ui->setSyncRateBtn->setDisabled(true);
        ui->syncRateLabel->setDisabled(true);
    } else {
        grid->setIsFullyAsync(false);
        ui->syncRateEdit->setDisabled(false);
        ui->setSyncRateBtn->setDisabled(false);
        ui->syncRateLabel->setDisabled(false);
    }
}


void MainWindow::on_gameBox_activated(int index)
{
    qDebug()<< "on_gameBox_activated:" << index;
    if (index == 6) {
        bool ok;
        QRegularExpression ruleExp("B[0-8]{1,8}/S[0-8]{1,8}");

        QString text = QInputDialog::getText(this, tr("Input BS Rule"),
                                             tr("Input like: B3/S23"), QLineEdit::Normal,
                                             "", &ok, Qt::MSWindowsFixedSizeDialogHint);

        if (ok && !text.isEmpty()) {
            if (ruleExp.match(text).hasMatch()) {
                qDebug() << "User input:" << text;
                grid->changeGame(text);
            } else {
                QMessageBox::critical(this, tr("Error"), tr("Invalid input. Please enter a rule in the format Bx/Sy."));
            }
        }
    }
}


void MainWindow::on_rowSlider_valueChanged(int value)
{
    qDebug() << "on_rowSlider_valueChanged";
    ui->rowDisplay->setText("Rows: " + QString::number(value));
    grid->changeRow(value);
}


void MainWindow::on_collumSlider_valueChanged(int value)
{
    qDebug() << "on_collumSlider_valueChanged";
    ui->collumDisplay->setText("Collums: " + QString::number(value));
    grid->changeCollum(value);
}


void MainWindow::on_rowDisplay_linkActivated(const QString &link)
{
    qDebug() << "on_rowDisplay_linkActivated ....";
}


void MainWindow::on_maxGenerationsSetBtn_clicked()
{
    int value = ui->maxGenerationsEdit->text().toInt();
    grid->setMaxGenerations(value);
}

void MainWindow::on_runTestBtn_clicked()
{
    grid->runTest1();
//    grid->runTestP0AndSyncRateAndDensity();
}

