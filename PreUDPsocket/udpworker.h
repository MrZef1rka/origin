#ifndef UDPWORKER_H
#define UDPWORKER_H


#include <QObject>
#include <QUdpSocket>
#include <QNetworkDatagram>
#include <QDateTime>
#include <QDataStream>


#define BIND_PORT 12345
#define TIMER_DELAY 100

class UDPworker : public QObject
{
    Q_OBJECT
public:
    explicit UDPworker(QObject *parent = nullptr);
    void InitSocket( void );

public slots:
    void SendDatagram(QByteArray data );
    void SendMessage(QString message);


private slots:
    void readPendingDatagrams(void);
    void ReadDatagram( QNetworkDatagram datagram);

private:
    QUdpSocket* serviceUdpSocket;

signals:
    void sig_sendTimeToGUI(QDateTime data);
    void sig_sendMessageInfoToGUI(QString info, QHostAddress sender, quint16 port, qint64 size);

};

#endif // UDPWORKER_H
