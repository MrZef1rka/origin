#ifndef STOPWATCH_H
#define STOPWATCH_H

#include <QObject>
#include <QTimer>

struct LapInfo {
    int number;
    int time;
    QString toString() const {
        int seconds = (time / 1000) % 60;
        int minutes = (time / 1000) / 60;
        int hundredths = (time % 1000) / 10;
        return QString("Круг %1, время: %2:%3:%4")
            .arg(number)
                        .arg(minutes, 2, 10, QChar('0'))
                        .arg(seconds, 2, 10, QChar('0'))
                        .arg(hundredths, 2, 10, QChar('0'));
    }
};

class Stopwatch : public QObject
{
    Q_OBJECT
public:
    explicit Stopwatch(QObject *parent = nullptr);

    void start();
    void stop();
    void clear();
    void lap();
    bool isRunning() const;
    LapInfo getLastLap() const;

signals:
    void timeUpdated(int time);            // Сигнал для обновления времени
    void lapUpdated(int lap, int lapTime); // Сигнал для обновления круга

private slots:
    void updateTimer();

private:
    QTimer *m_timer;
    int m_time;         // Общее время с момента старта
    int m_lastLapTime;  // Время последнего круга
    int m_lap;          // Номер текущего круга
    bool m_isRunning;   // Флаг для проверки работы секундомера
    LapInfo m_lastLap;
};

#endif // STOPWATCH_H
