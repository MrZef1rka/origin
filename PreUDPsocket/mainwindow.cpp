#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    udpWorker = new UDPworker(this);
    udpWorker->InitSocket();

    connect(udpWorker, &UDPworker::sig_sendTimeToGUI, this, &MainWindow::DisplayTime);
    connect(udpWorker, &UDPworker::sig_sendMessageInfoToGUI, this, &MainWindow::DisplayMessageInfo);

    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, [&]{

        QDateTime dateTime = QDateTime::currentDateTime();

        QByteArray dataToSend;
        QDataStream outStr(&dataToSend, QIODevice::WriteOnly);

        outStr << dateTime;

        udpWorker->SendDatagram(dataToSend);
    });

}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_pb_start_clicked()
{
    timer->start(TIMER_DELAY);
}


void MainWindow::DisplayTime(QDateTime data)
{
    counterPck++;
    if(counterPck % 20 == 0){
        ui->te_result->clear();
    }

    ui->te_result->append("Текущее время: " + data.toString() + ". "
                "Принято пакетов " + QString::number(counterPck));


}


void MainWindow::on_pb_stop_clicked()
{
    timer->stop();
}

void MainWindow::DisplayMessageInfo(QString info, QHostAddress sender, quint16 port, qint64 size)
{
    ui->te_result->append(info);
}

// НОВЫЙ МЕТОД ДЛЯ ОТПРАВКИ СООБЩЕНИЯ
void MainWindow::on_pb_sendMessage_clicked()
{
    QString message = ui->le_message->text();
    if (!message.isEmpty()) {
        udpWorker->SendMessage(message);
        ui->le_message->clear();
    }
}

