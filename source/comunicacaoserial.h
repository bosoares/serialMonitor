#ifndef COMUNICACAOSERIAL_H
#define COMUNICACAOSERIAL_H

#include <QSerialPort>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <QDebug>

class comunicacaoSerial
{
public:
   comunicacaoSerial();
   comunicacaoSerial(QSerialPort *serialDevice);
    // QSerialPort *serial;
    QStringList loadDevices();
    bool createConnection(QString device, uint32_t baudRate);
    void closeConnection();
protected:
    QSerialPort *myDevice;
};

#endif // COMUNICACAOSERIAL_H
