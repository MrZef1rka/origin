#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    m_stopwatch(new Stopwatch(this))
{
    ui->setupUi(this);

    ui->lapBrowser->setAlignment(Qt::AlignCenter);

    // Подключение сигналов и слотов
    connect(m_stopwatch, &Stopwatch::timeUpdated, this, &MainWindow::updateTimeDisplay);

    // Инициализация кнопок
    connect(ui->startStopButton, &QPushButton::clicked, this, &MainWindow::toggleStartStop);
    connect(ui->lapButton, &QPushButton::clicked, this, &MainWindow::handleLap);
    connect(ui->clearButton, &QPushButton::clicked, this, &MainWindow::clearStopwatch);

    // Кнопка «Круг» неактивна при запуске
    ui->lapButton->setEnabled(false);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::toggleStartStop()
{
    if (m_stopwatch->isRunning()) {
        m_stopwatch->stop();
        ui->startStopButton->setText("Старт");
        ui->lapButton->setEnabled(false); // Деактивируем кнопку «Круг»
    } else {
        m_stopwatch->start();
        ui->startStopButton->setText("Стоп");
        ui->lapButton->setEnabled(true);  // Активируем кнопку «Круг»
    }
}

void MainWindow::clearStopwatch()
{
    m_stopwatch->clear();
    ui->timeLabel->setText("0:00:00");      // Сбрасываем текст лейбла времени
    ui->lapBrowser->clear();             // Очищаем текстовый браузер
    ui->lapBrowser->setAlignment(Qt::AlignCenter);

}

void MainWindow::handleLap()
{
    m_stopwatch->lap();
    LapInfo lapInfo = m_stopwatch->getLastLap();
    ui->lapBrowser->append(lapInfo.toString());
}

void MainWindow::updateTimeDisplay(int time)
{
    int totalSeconds = time / 1000;
    int seconds = totalSeconds % 60;
    int minutes = totalSeconds / 60;
    int hundredths = (time % 1000) / 10; // Десятые доли секунды

    ui->timeLabel->setText(QString("%1:%2:%3")
                               .arg(minutes, 2, 10, QChar('0'))
                               .arg(seconds, 2, 10, QChar('0'))
                               .arg(hundredths, 2, 10, QChar('0')));
}

