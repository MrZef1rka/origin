#ifndef STOPWATCH_H
#define STOPWATCH_H

#include <QObject>
#include <QTimer>

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
};

#endif // STOPWATCH_H
