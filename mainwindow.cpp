#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->discoveryAgent = new QBluetoothDeviceDiscoveryAgent(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::captureDeviceProperties(const QBluetoothDeviceInfo &device)
{
    ui->comboBox_Devices->addItem(device.name() + device.address().toString());
}


void MainWindow::on_pushButton_Search_clicked()
{
    qDebug() << "Searching...";
    this->discoveryAgent->start();
    connect(this->discoveryAgent, SIGNAL(deviceDiscovered(QBluetoothDeviceInfo)), this, SLOT(captureDeviceProperties(QBluetoothDeviceInfo)));
}

void MainWindow::on_pushButton_Connect_clicked()
{
    qDebug() << "Connecting...";
}

void MainWindow::on_pushButton_Disconnect_clicked()
{
    qDebug() << "Disconnecting...";
}

void MainWindow::on_pushButton_Close_clicked()
{
    qDebug() << "Closing...";
}

void MainWindow::on_pushButton_LED_On_clicked()
{
    qDebug() << "LED on...";
}

void MainWindow::on_pushButton_LED_Off_clicked()
{
    qDebug() << "LED off...";
}
