#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSerialPort>
#include "comunicacaoserial.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void addPoint(double y);
    void plot();

private slots:
    void serialReceived();

    void on_pushButton_clicked();

private:
    Ui::MainWindow *ui;
    comunicacaoSerial *comunicacaoSerial_;
    QVector<double> qv_x{0}, qv_y{0};
 //   comunicacaoSerial *comunicacaoSerial;
};
#endif // MAINWINDOW_H
