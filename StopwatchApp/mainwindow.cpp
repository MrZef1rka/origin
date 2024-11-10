#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    m_stopwatch(new Stopwatch(this))
{
    ui->setupUi(this);

    // Подключение сигналов и слотов
    connect(m_stopwatch, &Stopwatch::timeUpdated, this, &MainWindow::updateTimeDisplay);
    connect(m_stopwatch, &Stopwatch::lapUpdated, this, &MainWindow::updateLapDisplay);

    // Инициализация кнопок
    connect(ui->startStopButton, &QPushButton::clicked, this, &MainWindow::toggleStartStop);
    connect(ui->lapButton, &QPushButton::clicked, m_stopwatch, &Stopwatch::lap);
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
    ui->timeLabel->setText("0:00");      // Сбрасываем текст лейбла времени
    ui->lapBrowser->clear();             // Очищаем текстовый браузер
}

void MainWindow::updateTimeDisplay(int time)
{
    int seconds = (time / 1000) % 60;
    int minutes = (time / 1000) / 60;
    ui->timeLabel->setText(QString("%1:%2").arg(minutes).arg(seconds, 2, 10, QChar('0')));
}

void MainWindow::updateLapDisplay(int lap, int lapTime)
{
    int seconds = (lapTime / 1000) % 60;
    int minutes = (lapTime / 1000) / 60;
    ui->lapBrowser->append(QString("Круг %1, время: %2:%3")
                               .arg(lap)
                               .arg(minutes)
                               .arg(seconds, 2, 10, QChar('0')));
}
