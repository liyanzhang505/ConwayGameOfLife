#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "mygridwidget.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , grid(new MyGridWidget(this))
{
    ui->setupUi(this);
    ui->gridLayout->addWidget(grid);
    ui->widgetLayout->setStretchFactor(ui->gridLayout, 8);
    ui->widgetLayout->setStretchFactor(ui->pannelLayout, 2);

}


MainWindow::~MainWindow()
{
    delete ui;
}

