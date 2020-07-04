#include "filecontrol.h"


fileControl::fileControl()
{
}

void fileControl::write(QVector<double>* data)
{
    QFileDialog *dialogBox = new QFileDialog();

//    QFile file(QDir::currentPath() + "/output/teste.txt");

    QString fileName = QFileDialog::getSaveFileName(dialogBox,
                                                    "Save serial data", //caption
                                                    "",                 //directory
                                                    "(*.txt);;All Files (*)"); //filter
    QFile file(fileName);
    qDebug() << "[fileControl] "  << fileName << '\n';

    if(file.open(QIODevice::WriteOnly) | QIODevice::Text)
    {
        QTextStream stream(&file);

        for(auto &buffer: *data)
        {
            stream << buffer <<'\n';
        }

        file.flush();
        file.close();
        qDebug() << "[fileControl] Data saved to file";
    }
    else
    {
        qDebug() << "[fileControl] Error - Not possible to save file";
    }


    delete dialogBox;

}
