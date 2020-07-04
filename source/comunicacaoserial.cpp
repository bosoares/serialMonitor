#include "comunicacaoserial.h"
#include <QDebug>

comunicacaoSerial::comunicacaoSerial(QSerialPort *serialDevice)
{
    myDevice = serialDevice;
}

QStringList comunicacaoSerial::loadDevices()
{
    QStringList devs;

    foreach (const QSerialPortInfo info, QSerialPortInfo::availablePorts()) {

        myDevice->setPort(info);


        if (myDevice->open(QIODevice::ReadOnly)) {
            myDevice->close();
            devs << info.portName();
            myDevice->clear();
        }

    }
    return devs;
}

bool comunicacaoSerial::createConnection(QString device, uint32_t baudRate)
{
    myDevice->setPortName(device);
    myDevice->setDataBits(QSerialPort::Data8);
    myDevice->setParity(QSerialPort::NoParity);
    myDevice->setStopBits(QSerialPort::OneStop);
    myDevice->setFlowControl(QSerialPort::NoFlowControl);

    if(myDevice->open(QIODevice::ReadOnly))
    {
        qDebug() << "[comunicacaoSerial] Serial communication has been established";
        switch (baudRate)
        {
            case 9600:
                 myDevice->setBaudRate(QSerialPort::Baud9600);
                 qDebug() << "[comunicacaoSerial] Set BaudRate to 9600 bps";
                break;
            case 19200:
                 myDevice->setBaudRate(QSerialPort::Baud19200);
                 qDebug() << "[comunicacaoSerial] Set BaudRate 19200 to bps";
                break;
            case 57600:
                 myDevice->setBaudRate(QSerialPort::Baud57600);
                 qDebug() << "[comunicacaoSerial] Set BaudRate 57600 to bps";
                break;
            case 115200:
                 myDevice->setBaudRate(QSerialPort::Baud115200);
                 qDebug() << "[comunicacaoSerial] Set BaudRate to 115200 bps";
                break;
            default:
            // Should never happens. Cases must be add if any value is add to cb_baudRate.
                qDebug() << "[comunicacaoSerial] Invalid baudRate:" << baudRate;
                qDebug() << "[comunicacaoSerial] Preparing to close connection";
                myDevice->clear();
                myDevice->close();
                qDebug() << "[comunicacaoSerial] Connection closed";
                return false;
        }
        qDebug() << "[comunicacaoSerial] Current bd:" << myDevice->baudRate();
        return true;
    }
    else
    {
        qDebug() << "[comunicacaoSerial] Serial communication fail";
        qDebug() << "Erro: " << myDevice->error();
        return false;
    }
}

bool comunicacaoSerial::closeConnection()
{
    qDebug() << "[comunicacaoSerial] Preparing to close connection";
    myDevice->clear();
    myDevice->close();

    if(!myDevice->isOpen()) //connection NOT open
    {
        qDebug() << "[comunicacaoSerial] Connection closed";
        return true;
    }
    else
    {
        qDebug() << "[comunicacaoSerial] Fail to close connection";
        return false;
    }

}
