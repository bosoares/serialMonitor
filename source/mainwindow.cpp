#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QSerialPort>
#include <QDebug>


QSerialPort *serial;
QCustomPlot *graphic;
//comunicacaoSerial *comunicacaoSerial_ = comunicacaoSerial_->getInstance();

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
/********************************************
*              Graphic stuff
******************************************/
  //  graphic = new QCustomPlot(this);

    //graphic->addGraph();
    //graphic->graph(0)->setPen(QPen(Qt::blue));
    ui->plot->addGraph();
    ui->plot->graph(0)->setPen(QPen(Qt::green));
    ui->plot->xAxis->setRange(0,1000);
    ui->plot->yAxis->setRange(-1,1);

    ui->plot->xAxis->grid()->setPen(QPen(QColor(140, 140, 140), 1, Qt::DotLine));
    ui->plot->yAxis->grid()->setPen(QPen(QColor(140, 140, 140), 1, Qt::DotLine));

    QLinearGradient plotGradient;
    plotGradient.setStart(0,-1);
    plotGradient.setFinalStop(1000, 1);
    plotGradient.setColorAt(0, QColor(80, 80, 80));
    plotGradient.setColorAt(1, QColor(50, 50, 50));
    ui->plot->setBackground(plotGradient);

    QLinearGradient axisRectGradient;
    axisRectGradient.setStart(0, -1);
    axisRectGradient.setFinalStop(1000, 1);
    axisRectGradient.setColorAt(0, QColor(80, 80, 80));
    axisRectGradient.setColorAt(1, QColor(30, 30, 30));
    ui->plot->axisRect()->setBackground(axisRectGradient);

/********************************************
 *      Serial stuff
 ******************************************/
    serial = new QSerialPort(this);
    comunicacaoSerial_ = new comunicacaoSerial(serial);
    ui->cb_serialDevices->addItems(comunicacaoSerial_->loadDevices());
    connect(serial,SIGNAL(readyRead()), this, SLOT(serialReceived()));


}

MainWindow::~MainWindow()
{
    delete ui;
    serial->close();
}

void MainWindow::addPoint(double y)
{
    qv_y.append(y);
    qv_x.append(qv_x.last()+3);
    ui->tx_receivedData->append(QString::number(y));
}

void MainWindow::plot()
{
    ui->plot->graph(0)->setData(qv_x,qv_y);
    if(qv_x.last()>1000)
    {
        double buffer;
        buffer = qv_y.last();
        qv_y.clear();
        qv_y.push_back(buffer);
        qv_x.clear();
        qv_x.push_back(0);
     }
        //ui->plot->graph(0)->rescaleAxes();
    ui->plot->replot();
    ui->plot->update();
}

void MainWindow::serialReceived(){
    ui->label->setText(serial->readLine());
    //ui->tx_receivedData;
    addPoint(ui->label->text().toDouble());
    plot();
    qDebug()<< "no data received";
}

void MainWindow::on_pushButton_clicked()
{
    comunicacaoSerial_->createConnection(ui->cb_serialDevices->currentText(),ui->cb_baudRate->currentText().toInt());
    //serial->setPortName(ui->cb_serialDevices->currentText());
}
