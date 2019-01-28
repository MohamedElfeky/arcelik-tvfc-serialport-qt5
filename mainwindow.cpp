#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QSerialPort>          // To use the serial port in your application
#include <QSerialPortInfo>      // For information about available serial ports
#include <QDebug>
#include <QMessageBox>
#include <QPalette>
#include <QTextCursor>
#include <QString>

static QSerialPort objSerialPort;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Start: turgaykale | Serial port info details and check connected ports
    int portnums = 0;

    qDebug() << "Number of serial ports:" << QSerialPortInfo::availablePorts().count();

    foreach (const QSerialPortInfo &serialPortInfo, QSerialPortInfo::availablePorts())
    {
        qDebug() <<  "\nPort:" << serialPortInfo.portName();
        qDebug() << "Location:" << serialPortInfo.systemLocation();
        qDebug() << "Description:" << serialPortInfo.description();
        qDebug() << "Manufacturer:" << serialPortInfo.manufacturer();
        qDebug() << "Vendor Identifier:" << (serialPortInfo.hasVendorIdentifier() ? QByteArray::number(serialPortInfo.vendorIdentifier(), 16) : QByteArray());
        qDebug() << "Product Identifier:" << (serialPortInfo.hasProductIdentifier() ? QByteArray::number(serialPortInfo.productIdentifier(), 16) : QByteArray());
        qDebug() << "Busy:" << (serialPortInfo.isBusy() ? QObject::tr("Yes") : QObject::tr("No"));

        if(serialPortInfo.manufacturer() == "FTDI")
        {
            QString tmpstr = serialPortInfo.systemLocation();
            this->ui->comboBox_Port->insertItem(portnums, tmpstr);
            portnums++;
        }

        QSerialPort *port = new QSerialPort(serialPortInfo);

        if (port->open(QIODevice::ReadWrite))
        {
            qDebug() << "Baud rate:" << port->baudRate();
            qDebug() << "Data bits:" << port->dataBits();
            qDebug() << "Stop bits:" << port->stopBits();
            qDebug() << "Parity:" << port->parity();
            qDebug() << "Flow control:" << port->flowControl();
            qDebug() << "Read buffer size:" << port->readBufferSize();
            port->close();
        }
        else
        {
            qDebug() << "Unable to open port, error code" << port->error();
        }
        delete port;
    }
    // Finish: turgaykale | Serial port info details and check connected ports

    // create obj. for Serial FTDI device
    this->ftdi_serial_port = new QSerialPort;
    this->serial_connection_status = EN_SERIALPORT_DISCONNECTED;
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_Connect_clicked()
{
    // connect / disconnect button

    qDebug() << "objSerialPort.BaudRate: " << ui->comboBox_Baudrate->currentText();
    qDebug() << "objSerialPort.PortName: " << ui->comboBox_Port->currentText();
    qDebug() << "this->serial_connection_status: " << this->serial_connection_status;

    // serial port configuration
    if(ui->comboBox_Port->currentText() != "" && this->serial_connection_status == EN_SERIALPORT_DISCONNECTED)
    {
        this->ftdi_serial_port->setPortName(ui->comboBox_Port->currentText());
        this->ftdi_serial_port->open(QSerialPort::ReadWrite);
        this->ftdi_serial_port->setBaudRate(QSerialPort::Baud115200);
        this->ftdi_serial_port->setDataBits(QSerialPort::Data8);
        this->ftdi_serial_port->setParity(QSerialPort::NoParity);
        this->ftdi_serial_port->setStopBits(QSerialPort::OneStop);
        this->ftdi_serial_port->setFlowControl(QSerialPort::NoFlowControl);

        this->serial_connection_status = EN_SERIALPORT_CONNECTED;

        // set button text to <Baglandi>
        ui->pushButton_Connect->setText("Bağlandı");

        // set buttons color to green
        QPalette pal = palette();
        pal.setColor(QPalette::Background, Qt::darkGreen);
        ui->pushButton_Connect->setAutoFillBackground(true);
        ui->pushButton_Connect->setPalette(pal);
    }
    else if(ui->comboBox_Port->currentText() == "")
    {
        QMessageBox::warning(this, "Port hatası", "FTDI aygıtı bulunamadı !");
        this->serial_connection_status = EN_SERIALPORT_DISCONNECTED;

        // set button text to <Baglandi>
        ui->pushButton_Connect->setText("Bağlan");

        // set buttons color to green
        QPalette pal = palette();
        pal.setColor(QPalette::Background, Qt::gray);
        ui->pushButton_Connect->setAutoFillBackground(true);
        ui->pushButton_Connect->setPalette(pal);
        ui->pushButton_Connect->show();
    }
    else if (ui->comboBox_Port->currentText() != "" && this->serial_connection_status == EN_SERIALPORT_CONNECTED)
    {
        // close serial port
        this->ftdi_serial_port->close();
        this->serial_connection_status = EN_SERIALPORT_DISCONNECTED;

        // set button text to <Baglandi>
        ui->pushButton_Connect->setText("Bağlan");

        // set buttons color to green
        QPalette pal = palette();
        pal.setColor(QPalette::Background, Qt::gray);
        ui->pushButton_Connect->setAutoFillBackground(true);
        ui->pushButton_Connect->setPalette(pal);
        ui->pushButton_Connect->show();
    }
    else
    {
        this->serial_connection_status = EN_SERIALPORT_DISCONNECTED;

        // set button text to <Baglandi>
        ui->pushButton_Connect->setText("Bağlan");
    }
}

void MainWindow::on_pushButton_Send_clicked()
{
    QTextCursor cursor(ui->textEdit_CommandArray->textCursor());
    const QString text_string = cursor.selectedText();

    qDebug() << "text_string: " << text_string;

    if(text_string == "")
    {
        // do nothing
        qDebug() << "Komut girilmedi !";
    }
    else
    {
        if(this->serial_connection_status == EN_SERIALPORT_CONNECTED && this->ftdi_serial_port->isWritable())
        {
            this->ftdi_serial_port->write("77");
            qDebug() << "Komut gönderildi !";
        }
    }
}
