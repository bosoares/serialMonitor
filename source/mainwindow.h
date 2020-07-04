#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDebug>
#include <QSerialPort>

#include "comunicacaoserial.h"
#include "filecontrol.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void clearData();
    void saveToPersistence();
    void setIcons();
    void setGraphicEnvironment();
    void addPoint(double y);
    void plot();

private slots:
    //Custom signals
    void serialReceived();

    //Layout elements
    void on_pb_connect_clicked();
    void on_pb_disconnect_clicked();

    void on_pb_save_clicked();

private:
    Ui::MainWindow *ui;
    QSerialPort *serial;
    comunicacaoSerial *comunicacaoSerial_;
    QVector<double> qv_x{0}, qv_y{0}, qv_y_persistence;
 //   comunicacaoSerial *comunicacaoSerial;
};
#endif // MAINWINDOW_H
