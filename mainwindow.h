#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QBluetoothDeviceDiscoveryAgent>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void captureDeviceProperties(const QBluetoothDeviceInfo &device);

    void searchFinished();

    void on_pushButton_Search_clicked();

    void on_pushButton_Connect_clicked();

    void on_pushButton_Disconnect_clicked();

    void on_pushButton_Close_clicked();

    void on_pushButton_LED_On_clicked();

    void on_pushButton_LED_Off_clicked();

private:
    Ui::MainWindow *ui;

    QBluetoothDeviceDiscoveryAgent *discoveryAgent;

    void addToLogs(QString message);
};
#endif // MAINWINDOW_H
