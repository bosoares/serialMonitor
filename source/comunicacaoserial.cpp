#include "comunicacaoserial.h"

comunicacaoSerial::comunicacaoSerial(QSerialPort *serialDevice)
{
    myDevice = serialDevice;
}

QStringList comunicacaoSerial::loadDevices()
{
    QStringList devs;

    foreach (const QSerialPortInfo info, QSerialPortInfo::availablePorts()) {

        myDevice->setPort(info);


        if (myDevice->open(QIODevice::ReadWrite)) {
            myDevice->close();
            devs << info.portName();
        }

    }
    return devs;
}

bool comunicacaoSerial::createConnection(QString device, uint32_t baudRate)
{
    myDevice->setPortName(device);
    switch (baudRate)
    {
        case 9600:
             myDevice->setBaudRate(QSerialPort::Baud9600);
            break;
        case 19200:
             myDevice->setBaudRate(QSerialPort::Baud19200);
            break;
        case 57600:
             myDevice->setBaudRate(QSerialPort::Baud57600);
            break;
        case 115200:
             myDevice->setBaudRate(QSerialPort::Baud115200);
            break;
    }
    myDevice->setBaudRate(QSerialPort::Baud9600);
    myDevice->setDataBits(QSerialPort::Data8);
    myDevice->setParity(QSerialPort::NoParity);
    myDevice->setStopBits(QSerialPort::OneStop);
    myDevice->setFlowControl(QSerialPort::NoFlowControl);
    myDevice->open(QIODevice::ReadWrite);
    return true;
}

void comunicacaoSerial::closeConnection()
{
    myDevice->clear();
    myDevice->close();
}
