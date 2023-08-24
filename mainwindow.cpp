#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "mygridwidget.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , grid(new MyGridWidget(this))
{
    ui->setupUi(this);
//    grid->setStyleSheet("background-color: white;");
//    grid->show();
    ui->gridLayout->addWidget(grid);
    ui->widgetLayout->setStretchFactor(ui->gridLayout, 8);
    ui->widgetLayout->setStretchFactor(ui->pannelLayout, 2);

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

