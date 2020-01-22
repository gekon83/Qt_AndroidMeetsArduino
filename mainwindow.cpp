#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDebug>
#include <QDateTime>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->discoveryAgent = new QBluetoothDeviceDiscoveryAgent(this);

    connect(this->discoveryAgent, SIGNAL(deviceDiscovered(QBluetoothDeviceInfo)), this, SLOT(captureDeviceProperties(QBluetoothDeviceInfo)));
    connect(this->discoveryAgent, SIGNAL(finished()), this, SLOT(searchFinished()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::captureDeviceProperties(const QBluetoothDeviceInfo &device)
{
    ui->comboBox_Devices->addItem(device.name() + " " + device.address().toString());
    //qDebug() << "device found. name: " << device.name() << " and address: " << device.address().toString();
    this->addToLogs("device found...\n  name/address: " + device.name() + " / " + device.address().toString());
}

void MainWindow::searchFinished()
{
    ui->pushButton_Search->setEnabled(true);
    this->addToLogs("Search finished.");
}


void MainWindow::on_pushButton_Search_clicked()
{
    //qDebug() << "Searching...";
    this->addToLogs("Search started.");
    ui->pushButton_Search->setEnabled(false);
    ui->comboBox_Devices->clear();

    this->discoveryAgent->start();
    //connect(this->discoveryAgent, SIGNAL(deviceDiscovered(QBluetoothDeviceInfo)), this, SLOT(captureDeviceProperties(QBluetoothDeviceInfo)));
}

void MainWindow::on_pushButton_Connect_clicked()
{
    //qDebug() << "Connecting...";
    this->addToLogs("Initialising connection.");
}

void MainWindow::on_pushButton_Disconnect_clicked()
{
    //qDebug() << "Disconnecting...";
    this->addToLogs("Closing connection.");
}

void MainWindow::on_pushButton_Close_clicked()
{
    //qDebug() << "Closing...";
    this->addToLogs("Closing app.");
}

void MainWindow::on_pushButton_LED_On_clicked()
{
    //qDebug() << "LED on...";
    this->addToLogs("Turning on LED.");
}

void MainWindow::on_pushButton_LED_Off_clicked()
{
    //qDebug() << "LED off...";
    this->addToLogs("Turning off LED.");
}

void MainWindow::addToLogs(QString message)
{
    //QString currentDateTime = QDateTime::currentDateTime().toString("yyyy.MM.dd hh:mm:ss");
    QString currentDateTime = QDateTime::currentDateTime().toString("hh:mm:ss");
    ui->textEdit_Log->append(currentDateTime + "\t" + message);
}
