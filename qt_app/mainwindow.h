#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QDebug>
#include <QTimer>
#include <QMessageBox>
#include <QFile>
#include <QTextStream>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

enum eFrameType
{
    FrameType_Header = 155,
    FrameType_Null = 0,
    FrameType_DriveGeneral = 1,
    FrameType_ManipGeneral = 2,
    FrameType_LabGeneral = 3,
    FrameType_ManipCmd = 4,
    FrameType_ManipSave = 5,
    FrameType_LabCmd = 6,
    FrameType_ServodriveTelemetry = 9,
    FrameType_DriveCmd = 10,
    FrameType_MainboardToDriveGenerallSpeed = 11,
    FrameType_ManipFeedbackMotpos = 12,
    Frametype_LabFeedbackMotPos = 13,
    FrameType_DriveStatusTelemetry = 14,
    FrameType_ManipStatusTelemetry = 15,
    FrameType_LabStatusTelemetry = 16,
    FrameType_RoverPosABS = 20,
    FrameType_NvidiaURC = 21,
    FrameType_NewpointsABS = 22,
    FrameType_NewpointsREL = 23,
    FrameType_ArtagPosABS = 24,
    FrameType_ArtagPosREL = 25,
    FrameType_RoverPosREL = 30,
    FrameType_NvidiaERC = 31,
    FrameType_NewpointsERC = 32,
    FrameType_SetYawAndPos = 33,
    FrameType_ManipFeedbackAxesposMat = 34,
    FrameType_ManipFeedbackAxesposRPY = 35,
    FrameType_ManipJoinMoveAbs = 36,
    FrameType_ManipJoinMoveInc = 37,
    FrameType_ManipBaseMoveAbs = 38,
    FrameType_ManipBaseMoveInc = 39,
    FrameType_ManipToolMoveInc = 40,
    FrameType_ManipGripperMove = 41,
    FrameType_ManipBaseMoveAbsMat = 42,
    FrameType_ManipBaseMoveIncMat = 43,
    FrameType_ManipToolMoveIncMat = 44,
    Frametype_EmergencyHardStop = 201,
};
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_openport1button_clicked();

    void on_openport2button_clicked();

    void on_closeport1button_clicked();

    void on_closeport2button_clicked();

    void serial1send(const char data);

    void serial2send(const char data);

    void read1();

    void read2();

    void on_confirm_add_user_clicked();

    void on_open_all_button_clicked();

    void on_close_all_button_clicked();

    void on_remove_all_button_clicked();

    void search_file(QString linea);

    void on_remove_user_button_clicked();


private:
    Ui::MainWindow *ui;
    QSerialPort serial1;
    QSerialPort serial2;
    QTimer *timerSerial;
    QTimer *timerLabel;
    eFrameType frameToSendBySerial;
    QString serial1buf;
    QString serial2buf;
    QByteArray bufread1;
    QByteArray bufread2;
    QString username;
    uint8_t buf[100];
    uint8_t user;
    QString text;
    uint8_t deluser;

};
#endif // MAINWINDOW_H
