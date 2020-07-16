#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QSerialPort>
#include <QDebug>
#include <iostream>
#include <string>

/* **********************************************************************
 *              Constructor & Destructor
 * **********************************************************************/
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    //Initialize objects
    ui->setupUi(this);

    serial = new QSerialPort(this);
    comunicacaoSerial_ = new comunicacaoSerial(serial);

    //Setup Graphic environment
    setGraphicEnvironment();

    //Fill comboBoxes options
    setIcons();
    ui->cb_serialDevices->addItems(comunicacaoSerial_->loadDevices());

    //Set signal(s)
    connect(serial,SIGNAL(readyRead()), this, SLOT(serialReceived()));


}

MainWindow::~MainWindow()
{
    delete ui;
    serial->close();
}

/* **********************************************************************
 *              QtCustom plot configuration (graphic)
 * **********************************************************************/

//Configure grapgic style and parameters
void MainWindow::setIcons()
{
    //ui->pb_save->setIcon(QIcon("/home/bossoares/Documents/Programação/Embarcados/serialMonitor/source/images/Save-icon.png"));
    ui->pb_save->setIcon(QIcon("./images/Save-icon.png"));
    qDebug() << "Set icon" << QDir::currentPath() ;
}
void MainWindow::setGraphicEnvironment()
{
    ui->plot->addGraph();
    ui->plot->graph(0)->setPen(QPen(Qt::green));
    ui->plot->xAxis->setRange(0,1000);
    ui->plot->yAxis->setRange(0,255);

    ui->plot->yAxis->setLabelColor(Qt::white);
    ui->plot->xAxis->setLabelColor(Qt::white);

    ui->plot->xAxis->setLabel("Time");
    ui->plot->yAxis->setLabel("Amplitude [v]");

    ui->plot->xAxis->setBasePen(QPen(Qt::white, 1));
    ui->plot->yAxis->setBasePen(QPen(Qt::white, 1));
    ui->plot->xAxis->setTickPen(QPen(Qt::white, 1));
    ui->plot->yAxis->setTickPen(QPen(Qt::white, 1));
    ui->plot->xAxis->setSubTickPen(QPen(Qt::white, 1));
    ui->plot->yAxis->setSubTickPen(QPen(Qt::white, 1));
//    ui->plot->xAxis->setTickLabelColor(Qt::white);
    ui->plot->xAxis->setTickLabels(false);
    ui->plot->yAxis->setTickLabelColor(Qt::white);

    ui->plot->xAxis->grid()->setPen(QPen(QColor(140, 140, 140), 1, Qt::DotLine));
    ui->plot->yAxis->grid()->setPen(QPen(QColor(140, 140, 140), 1, Qt::DotLine));

    ui->plot->xAxis->grid()->setSubGridPen(QPen(QColor(80, 80, 80), 1, Qt::DotLine));
    ui->plot->yAxis->grid()->setSubGridPen(QPen(QColor(80, 80, 80), 1, Qt::DotLine));
    ui->plot->xAxis->grid()->setSubGridVisible(true);
    ui->plot->yAxis->grid()->setSubGridVisible(true);
    ui->plot->xAxis->grid()->setZeroLinePen(Qt::NoPen);
    ui->plot->yAxis->grid()->setZeroLinePen(Qt::NoPen);

    //ui->plot->yAxis->setUpperEnding(QCPLineEnding::esSpikeArrow);
    ui->plot->xAxis->setUpperEnding(QCPLineEnding::esSpikeArrow);

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

}

/* **********************************************************************
 *              Data display
 * **********************************************************************/

//Plot and display data in the graphic
void MainWindow::plot()
{
    ui->plot->graph(0)->setData(qv_x,qv_y);
    if(qv_x.last()>1000) //Clear the screen when it hit the end of the chart
    {
        saveToPersistence();
    }
    ui->plot->replot();
    ui->plot->update();
}
/* **********************************************************************
 *              Data handler
 * **********************************************************************/
void MainWindow::saveToPersistence()
{
    qv_y_persistence.append(qv_y); //save historical data to persistence file
    qv_y.clear();
    qv_y.push_back(qv_y_persistence.last()); //plot() need a valid point
    qv_y_persistence.pop_back(); //remove last element, otherwise it will be duplicated

    qv_x.clear();
    qv_x.push_back(0);
}

void MainWindow::clearData()
{
    qv_y_persistence.clear();
    qv_y.clear();
    qv_x.push_back(0);
    qv_x.clear();
    qv_x.push_back(0);
}
/* **********************************************************************
 *              Serial communication
 * **********************************************************************/

//Handles serial reception signal
void MainWindow::serialReceived(){
    QString bufferSerial;
//    bufferSerial = serial->readLine();

    const QByteArray data = serial->readAll();
    for (int i = 0; i < data.size(); ++i) {
        bufferSerial = QObject::tr("%1").arg(uchar(data.at(i)));
        //ui->tx_receivedData->append(bufferSerial);
        qv_y.push_back(bufferSerial.toDouble());
        //qDebug() << qv_y.last();
        //qDebug() << bufferSerial.toInt();
    }
        //qDebug() << qv_y.last();
//    bool ok;
//    int num = std::stoi("123");
//    if (!ok) {
//        qDebug() << "Conversion failed!";
//      // conversion failed
//    }
//    else
//    {
//        qDebug() << num;
//    }
    addPoint(bufferSerial.toDouble());
    plot();
}

//Save data from serial port
void MainWindow::addPoint(double y)
{
    qv_y.append(y);
    qv_x.append(qv_x.last()+3);
    //ui->tx_receivedData->append(QString::number(y));
}


/* **********************************************************************
 *              Control Elements Actions (Buttons & etc)
 * **********************************************************************/
void MainWindow::on_pb_connect_clicked()
{
    if(comunicacaoSerial_->createConnection(ui->cb_serialDevices->currentText(),ui->cb_baudRate->currentText().toInt()))
    {
        ui->cb_serialDevices->setEnabled(false);
        ui->cb_baudRate->setEnabled(false);
        ui->pb_connect->setEnabled(false);
        ui->pb_save->setEnabled(false);
        ui->pb_disconnect->setEnabled(true);
        clearData();
    }
    else
    {
        qDebug() << "Connection not possible.";
    }
}

void MainWindow::on_pb_disconnect_clicked()
{
     if (comunicacaoSerial_->closeConnection())
     {
         ui->cb_serialDevices->setEnabled(true);
         ui->cb_baudRate->setEnabled(true);
         ui->pb_connect->setEnabled(true);
         ui->pb_save->setEnabled(true);
         ui->pb_disconnect->setEnabled(false);
     }
     else
     {
         qDebug() << "Connection not possible.";
     }
}

void MainWindow::on_pb_save_clicked()
{
    fileControl *fileControl_ = new fileControl();
    qDebug() << "[MainWindow] Request to save data to .txt";
    saveToPersistence();
    fileControl_->write(&qv_y_persistence);
}
