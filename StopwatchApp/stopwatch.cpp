#include "stopwatch.h"

Stopwatch::Stopwatch(QObject *parent)
    : QObject(parent), m_time(0), m_lastLapTime(0), m_lap(0), m_isRunning(false)
{
    m_timer = new QTimer(this);
    connect(m_timer, &QTimer::timeout, this, &Stopwatch::updateTimer);
}

void Stopwatch::start()
{
    if (!m_isRunning) {
        m_timer->start(100);  // Интервал обновления 0.1 сек
        m_isRunning = true;
    }
}

void Stopwatch::stop()
{
    if (m_isRunning) {
        m_timer->stop();
        m_isRunning = false;
    }
}

void Stopwatch::clear()
{
    //m_timer->stop();
    //m_isRunning = false;
    m_time = 0;
    m_lastLapTime = 0;
    m_lap = 0;
    emit timeUpdated(m_time);  // Сбрасываем отображаемое время
}

void Stopwatch::lap()
{
    if (m_isRunning) {
        int lapTime = m_time - m_lastLapTime; // Время с предыдущего круга
        m_lastLapTime = m_time;               // Обновляем время последнего круга
        m_lap++;
        emit lapUpdated(m_lap, lapTime);      // Отправляем сигнал для интерфейса
    }
}

bool Stopwatch::isRunning() const
{
    return m_isRunning;
}

void Stopwatch::updateTimer()
{
    m_time += 100; // Увеличиваем время на 0.1 сек
    emit timeUpdated(m_time);
}
