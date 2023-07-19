#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QDebug>
#include <QTimer>
#include <QMessageBox>
#include <QFile>
#include <QTextStream>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

     qDebug() << "Number of ports: " << QSerialPortInfo::availablePorts().length();
     foreach(const QSerialPortInfo &serialPortInfo, QSerialPortInfo::availablePorts()){

         ui->combobox1->addItem(serialPortInfo.portName());
         ui->combobox2->addItem(serialPortInfo.portName());
         timerSerial = new QTimer(this);

         frameToSendBySerial = FrameType_DriveGeneral;
     }
     serial1buf = "";
     serial2buf = "";

     user = 0;


}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_openport1button_clicked()
{
    serial1.setPortName(ui->combobox1->currentText());
    serial1.setBaudRate(QSerialPort::Baud19200);
    serial1.setDataBits(QSerialPort::Data8);
    serial1.setParity(QSerialPort::NoParity);
    serial1.setStopBits(QSerialPort::OneStop);
    if(!(serial1.isOpen()))
    {
        serial1.open(QIODevice::ReadWrite);
        if(serial1.isOpen())
        {
            connect(&serial1, SIGNAL(readyRead()),this, SLOT(read1()));
            ui->openport1button->setEnabled(false);
            ui->closeport1button->setEnabled(true);
            qDebug() << "The 1st COM port has been opened.";
        }
        else
        {
            qDebug() << "The 1st COM port has not been opened.";
        }
    }
    else
    {
        qDebug() << "The 1st COM port is already open.";
    }
}


void MainWindow::on_openport2button_clicked()
{
    serial2.setPortName(ui->combobox2->currentText());
    serial2.setBaudRate(QSerialPort::Baud19200);
    serial2.setDataBits(QSerialPort::Data8);
    serial2.setParity(QSerialPort::NoParity);
    serial2.setStopBits(QSerialPort::OneStop);
    if(!(serial2.isOpen()))
    {
        serial2.open(QIODevice::ReadWrite);
        if(serial2.isOpen())
        {
            connect(&serial2, SIGNAL(readyRead()),this, SLOT(read2()));
            ui->openport2button->setEnabled(false);
            ui->closeport2button->setEnabled(true);
            qDebug() << "The 2nd COM port has been opened.";
        }
        else
        {
            qDebug() << "The 2nd COM port has not been opened.";
        }
    }
    else
    {
        qDebug() << "The 2nd COM port is already open.";
    }
}


void MainWindow::on_closeport1button_clicked()
{
    serial1.close();
    if(!(serial1.isOpen()))
    {
        timerSerial->stop();
        ui->openport1button->setEnabled(true);
        ui->closeport1button->setEnabled(false);
        qDebug() << "The 1st COM port has been closed.";
    }
}


void MainWindow::on_closeport2button_clicked()
{
    serial2.close();
    if(!(serial2.isOpen()))
    {
        timerSerial->stop();
        ui->openport2button->setEnabled(true);
        ui->closeport2button->setEnabled(false);
        qDebug() << "The 2nd COM port has been closed.";
    }
}


void MainWindow::serial1send(const char data)
{
    if(serial1.isWritable())
        {
            serial1.write(&data, 1);
        }
    else
    {
        qDebug() << "Couldn't write to 1st serial.";
    }
}


void MainWindow::serial2send(const char data)
{
    if(serial2.isWritable())
        {
            serial2.write(&data, 1);
        }
    else
    {
        qDebug() << "Couldn't write to 2nd serial.";
    }
}


void MainWindow::read1()
{
    bufread1.append(serial1.readAll());
    qDebug() << "1st serial read:" << bufread1;
    if(bufread1.isNull())
    {
        qDebug() << "2nd serial NULL";
    }
    if(bufread1.size()>5)
    {
        for(int i=0;i<bufread1.size();i++)
        {
            buf[i] = static_cast<uint8_t>(bufread1[i]);
        }
        qDebug() << "Card ID" << buf[0] << " " << buf[1] << " " << buf[2] << " " << buf[3] << " " << buf[4] << " " << buf[5];

        QFile file("C:/Users/kubap/OneDrive/Desktop/qt_app/plik.txt");
        if(!file.exists())
        {
            file.open(QIODevice::ReadWrite);
            QTextStream out(&file);
            out.flush();
            file.close();
        }
        if(!(file.open(QIODevice::ReadOnly)))
        {
            QMessageBox::warning(this, "error", "ERROR");
        }
        QTextStream plik(&file);
        while(!plik.atEnd())
        {
            QString line = plik.readLine();
            search_file(line);
        }

        bufread1.clear();
    }
}


void MainWindow::read2()
{
    bufread2.append(serial2.readAll());
    qDebug() << "2nd serial read:" << bufread2;
    if(bufread2.isNull())
    {
        qDebug() << "2nd serial NULL";
    }
    if(bufread2.size()>5)
    {
        for(int i=0;i<bufread2.size();i++)
        {
            buf[i] = static_cast<uint8_t>(bufread2[i]);
        }
        qDebug() << "Card ID" << buf[0] << " " << buf[1] << " " << buf[2] << " " << buf[3] << " " << buf[4] << " " << buf[5];

        QFile file("C:/Users/kubap/OneDrive/Desktop/qt_app/plik.txt");
        if(!file.exists())
        {
            file.open(QIODevice::ReadWrite);
            QTextStream out(&file);
            out.flush();
            file.close();
        }
        if(!(file.open(QIODevice::ReadOnly)))
        {
            QMessageBox::warning(this, "error", "ERROR");
        }
        QTextStream plik(&file);
        while(!plik.atEnd())
        {
            QString line = plik.readLine();
            search_file(line);
        }

        bufread2.clear();
    }
}


void MainWindow::on_confirm_add_user_clicked()
{
    username = ui->lineEdit_name->text();
    qDebug() << "username:" << username;
    if(ui->lineEdit_name->text().isEmpty())
    {
        QMessageBox::warning(this,"Add New User", "No user added");
    }
    else
    {
        QMessageBox::information(this,"Add New User", "Added new user");
    }

    if(buf[0]==69)
    {
        QFile file("C:/Users/kubap/OneDrive/Desktop/qt_app/plik.txt");
        if(!file.exists())
        {
            file.open(QIODevice::ReadWrite);
            QTextStream out(&file);
            out.flush();
            file.close();
        }


        QTextStream out(&file);
        text = ui->lineEdit_name->text();
        user = ui->spinBox->value();

        if(file.open(QIODevice::WriteOnly | QIODevice::Append))
        {      
            out << user << " ";
            out << buf[1] << " " << buf[2] << " " << buf[3] << " " << buf[4] << " " << buf[5] << " ";
            out << text << "-\r\n";

        }

        out.flush();
        file.close();


        file.open(QFile::ReadOnly | QFile::Text);
        ui->plainTextEdit->setPlainText(file.readAll());
        file.close();

    }
    for(int i=0;i<100;i++)
        {
        buf[i] = 0;
        }

}


void MainWindow::on_open_all_button_clicked()
{
    serial1send('a');
    serial2send('a');
    QMessageBox::information(this,"Emergency","Opened all door");
}


void MainWindow::on_close_all_button_clicked()
{
    serial1send('b');
    serial2send('b');
    QMessageBox::information(this,"Emergency","Closed all door");
}


void MainWindow::on_remove_all_button_clicked()
{
    QFile file("C:/Users/kubap/OneDrive/Desktop/qt_app/plik.txt");
    if(!file.exists())
    {
        file.open(QIODevice::ReadWrite);
        QTextStream out(&file);
        out.flush();
        file.close();
    }
    file.resize(0);
    file.open(QFile::ReadOnly | QFile::Text);
    ui->plainTextEdit->setPlainText(file.readAll());
    file.close();
    QMessageBox::information(this,"User list", "Removed all users");
}


void MainWindow::search_file(QString linea)
{
    QStringList A = linea.split(" ");
    int a1 = A[0].toInt();
    int a2 = A[1].toInt();
    int a3 = A[2].toInt();
    int a4 = A[3].toInt();
    int a5 = A[4].toInt();
    int a6 = A[5].toInt();

    if(a2 == buf[1])
    {
        if(a3 == buf[2])
        {
            if(a4 == buf[3])
            {
                if(a5 == buf[4])
                {
                    if(a6 == buf[5])
                    {
                        switch (a1)
                        {
                            case 1:
                                serial1send('a');
                                serial2send('a');
                                break;
                            case 2:
                                serial1send('a');
                                serial2send('b');
                                break;
                            case 3:
                                serial1send('b');
                                serial2send('a');
                                break;
                            case 4:
                                serial1send('b');
                                serial2send('b');
                            break;
                        }
                    }
                }
            }
        }
    }
}


void MainWindow::on_remove_user_button_clicked()
{
    QFile file("C:/Users/kubap/OneDrive/Desktop/qt_app/plik.txt");
    deluser = ui->spinBox_2->value();
    int x = deluser - 1;
    file.open(QIODevice::ReadWrite);

    QTextStream plik(&file);

    QString f = plik.readAll();
    QList<QString> A = f.split("-");
    qDebug() << A;
    A.remove(x);
    qDebug() << A;
    int y = A.count() - 1;
    file.resize(0);
    for(int i=0;i<y;i++)
    {
    plik << A[i] << "-";
    }
    plik << A[y];

    plik.flush();
    file.close();

    ui->plainTextEdit->clear();

    file.open(QFile::ReadOnly | QFile::Text);
    ui->plainTextEdit->setPlainText(file.readAll());
    file.close();
    QMessageBox::information(this,"User list", "Removed user");

}























