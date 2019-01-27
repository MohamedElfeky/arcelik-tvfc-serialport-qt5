#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSerialPort>          // To use the serial port in your application

enum ConnectionStatus
{
    EN_SERIALPORT_DISCONNECTED = 0,
    EN_SERIALPORT_CONNECTED = 1
};

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    // custom field
    QSerialPort *ftdi_serial_port;
    ConnectionStatus serial_connection_status;

private slots:
    void on_pushButton_Connect_clicked();

    void on_pushButton_Send_clicked();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
