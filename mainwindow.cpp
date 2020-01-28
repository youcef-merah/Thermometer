#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "thermometer.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);    
    Thermometer * therm = new Thermometer(this);
    therm->setCurrentTemperature(7.9);
}

MainWindow::~MainWindow()
{
    delete ui;
}

