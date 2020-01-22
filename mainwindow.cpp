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
    this->socket = new QBluetoothSocket(QBluetoothServiceInfo::RfcommProtocol, this);

    connect(this->discoveryAgent, SIGNAL(deviceDiscovered(QBluetoothDeviceInfo)), this, SLOT(captureDeviceProperties(QBluetoothDeviceInfo)));
    connect(this->discoveryAgent, SIGNAL(finished()), this, SLOT(searchFinished()));

    connect(this->socket, SIGNAL(connected()), this, SLOT(connectionEstablished()));
    connect(this->socket, SIGNAL(disconnected()), this, SLOT(connectionInterrupted()));
    connect(this->socket, SIGNAL(readyRead()), this, SLOT(sockectReadyToRead()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::captureDeviceProperties(const QBluetoothDeviceInfo &device)
{
    ui->comboBox_Devices->addItem(device.name() + "  >>>  " + device.address().toString());
    //qDebug() << "device found. name: " << device.name() << " and address: " << device.address().toString();
    this->addToLogs("device found...\n  name/address: " + device.name() + " / " + device.address().toString());
}

void MainWindow::searchFinished()
{
    ui->pushButton_Search->setEnabled(true);
    this->addToLogs("Search finished.");
}

void MainWindow::connectionEstablished()
{
    this->addToLogs("Connection established.");
}

void MainWindow::connectionInterrupted()
{
    this->addToLogs("Connection interrupted.");
}

void MainWindow::sockectReadyToRead()
{
    while (this->socket->canReadLine()) {
        QString line = this->socket->readLine();
        qDebug() << line;

        QString terminator = "\r";
        int pos = line.lastIndexOf(terminator);
        qDebug() << line.left(pos);

        this->addToLogs(line.left(pos));
    }
}

void MainWindow::on_pushButton_Search_clicked()
{
    this->addToLogs("Search started.");
    ui->pushButton_Search->setEnabled(false);
    ui->comboBox_Devices->clear();

    this->discoveryAgent->start();
    //connect(this->discoveryAgent, SIGNAL(deviceDiscovered(QBluetoothDeviceInfo)), this, SLOT(captureDeviceProperties(QBluetoothDeviceInfo)));
}

void MainWindow::on_pushButton_Connect_clicked()
{
    if (ui->comboBox_Devices->count()) {
        this->addToLogs("Initialising connection.");
        QString comboBoxQString = ui->comboBox_Devices->currentText();
        QStringList portList = comboBoxQString.split("  >>>  ");
        QString deviceAddress = portList.last();

        static const QString serviceUuid(QStringLiteral("00001101-0000-1000-8000-00805F9B34FB"));
        this->socket->connectToService(QBluetoothAddress(deviceAddress), QBluetoothUuid(serviceUuid), QIODevice::ReadWrite);
        this->addToLogs("Connecting to device/address: " + portList.first() + " / " + portList.last());
    } else {
        this->addToLogs("No devices selected.");
    }
}

void MainWindow::on_pushButton_Disconnect_clicked()
{
    this->addToLogs("Closing connection.");
    this->socket->disconnectFromService();
}

void MainWindow::on_pushButton_Close_clicked()
{
    this->addToLogs("Closing app.");
}

void MainWindow::on_pushButton_LED_On_clicked()
{
    this->addToLogs("Turning on LED.");
    sendMessageToDevice("1");
}

void MainWindow::on_pushButton_LED_Off_clicked()
{
    //qDebug() << "LED off...";
    this->addToLogs("Turning off LED.");
    sendMessageToDevice("0");
}

void MainWindow::addToLogs(QString message)
{
    //QString currentDateTime = QDateTime::currentDateTime().toString("yyyy.MM.dd hh:mm:ss");
    QString currentDateTime = QDateTime::currentDateTime().toString("hh:mm:ss");
    ui->textEdit_Log->append(currentDateTime + "\t" + message);
}

void MainWindow::sendMessageToDevice(QString message)
{
    if (this->socket->isOpen() && this->socket->isWritable()) {
        this->addToLogs("Sending message to device" + message);
        this->socket->write(message.toStdString().c_str());
    } else {
        this->addToLogs("Cannot send message. No open connection.");
    }
}
