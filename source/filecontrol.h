#ifndef FILECONTROL_H
#define FILECONTROL_H

#include <QtCore>
#include <QtCore/QCoreApplication>
#include <QVector>
#include <QDebug>
#include <QFileDialog>

class fileControl
{
public:
    fileControl();
    void write(QVector<double>* data);
};

#endif // FILECONTROL_H
