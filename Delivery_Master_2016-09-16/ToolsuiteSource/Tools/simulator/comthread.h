#ifndef COMTHREAD_H
#define COMTHREAD_H

#include <QThread>
#include <QString>
#include <QTcpSocket>
#include <QHostAddress>

class ComThread : public QThread
{
    Q_OBJECT

public:
    ComThread(QHostAddress host = QHostAddress::LocalHost, quint16 port = 7085, QObject* parent = 0);
    ~ComThread();

public slots:
    void sendMessageToPCIC(const QString& json);

signals:
    void messageReceivedFromSimulator(const QByteArray& json);
    void signal_socketDisconnected(void);
    void signal_socketError(QAbstractSocket::SocketError error);

private:
    QTcpSocket* m_socket;
    QByteArray m_buffer;

    bool m_isConnected;
    bool m_messageStarted;
    int m_jsonBrackets;

    void socketConnected();
    void socketDisconnected();
    void socketReadyRead();
    void socketError(QAbstractSocket::SocketError error);

};

#endif // COMTHREAD_H
