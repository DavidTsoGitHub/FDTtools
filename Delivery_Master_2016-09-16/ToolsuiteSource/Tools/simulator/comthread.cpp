#include "comthread.h"

ComThread::ComThread(QHostAddress host, quint16 port, QObject* parent) : QThread(parent),
    m_socket(new QTcpSocket(this)),
    m_isConnected(false),
    m_messageStarted(false),
    m_jsonBrackets(0)
{
    m_socket->abort();
    m_socket->connectToHost(host, port);

    connect(m_socket, &QTcpSocket::connected, this, &ComThread::socketConnected);
    connect(m_socket, &QTcpSocket::disconnected, this, &ComThread::socketDisconnected);
    connect(m_socket, &QTcpSocket::disconnected, this, &ComThread::signal_socketDisconnected);
    connect(m_socket, &QTcpSocket::readyRead, this, &ComThread::socketReadyRead);
    connect(m_socket, static_cast<void(QTcpSocket::*)(QAbstractSocket::SocketError)>(&QTcpSocket::error), this, &ComThread::socketError);
    connect(m_socket, static_cast<void(QTcpSocket::*)(QAbstractSocket::SocketError)>(&QTcpSocket::error), this, &ComThread::signal_socketError);

    start();
}

ComThread::~ComThread()
{
    m_socket->abort();
}

void ComThread::sendMessageToPCIC(const QString& message)
{
    if (!m_isConnected) return;
    m_socket->write(message.toUtf8());
    m_socket->flush();
}

void ComThread::socketConnected()
{
    qDebug(__FUNCTION__);
    m_messageStarted = false;
    m_jsonBrackets = 0;
    m_isConnected = true;
}

void ComThread::socketDisconnected()
{
    qDebug(__FUNCTION__);
    m_isConnected = false;
    m_messageStarted = false;
    m_jsonBrackets = 0;
    m_buffer.clear();
}

void ComThread::socketReadyRead()
{
    qint64 availableBytes = m_socket->bytesAvailable();

    while (availableBytes > 0)
    {
        QByteArray buffer = m_socket->read(availableBytes);
        availableBytes -= buffer.size();

        for (int i = 0; i < buffer.count(); ++i)
        {
            if (buffer.at(i) == '{')
            {
                m_messageStarted = true;
                ++m_jsonBrackets;
            }
            else if (buffer.at(i) == '}')
            {
                if (m_jsonBrackets > 0)
                {
                    --m_jsonBrackets;
                }
            }

            if (m_messageStarted)
            {
                m_buffer.append(buffer.at(i));
            }

            if (m_messageStarted && m_jsonBrackets == 0)
            {
                emit messageReceivedFromSimulator(m_buffer);
                m_buffer.clear();
                m_messageStarted = false;
            }
        }

        availableBytes = m_socket->bytesAvailable();
    }

//    while (availableBytes >= 4) // We should be able to at least read a size indication message
//    {
//        if (m_expectedBytes == 0)
//        {   // Next four bytes should indicate an expected size of the following message
//            m_socket->read((char*)&m_expectedBytes, 4);
//            availableBytes -= 4;
//        }

//        if (m_expectedBytes > 0 && availableBytes > 0)
//        {   // Eat up the buffer until we have either read the full message or there is no more data to read
//            qint64 nbrOfBytes = qMin(m_expectedBytes, (int)availableBytes);
//            QByteArray chunk = m_socket->read(nbrOfBytes);
//            m_buffer.append(chunk);
//            m_expectedBytes -= chunk.size();
//            availableBytes -= chunk.size();

//            if (m_expectedBytes == 0)
//            {   // We have read the full message
//                emit messageReceivedFromSimulator(m_buffer);
//                m_buffer.clear();
//            }
//        }
//    }
}

void ComThread::socketError(QAbstractSocket::SocketError error)
{
    if (m_socket->state() != QAbstractSocket::ConnectedState)
    {
        m_isConnected = false;
    }

    qDebug("%s(%d) %s", __FUNCTION__, error, qUtf8Printable(m_socket->errorString() + ", retrying. (Check that PCIC is running)"));
}
