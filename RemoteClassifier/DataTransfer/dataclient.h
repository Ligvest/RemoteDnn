#pragma once

#include <QByteArray>
#include <QDataStream>
#include <QHostAddress>
#include <QTcpSocket>
#include <iostream>

/**
 * @brief DataClient is a QTcpSocket wrapper that allow you to connect to DataServer by TCP
 */
class DataClient : public QObject {
    Q_OBJECT

   public:
    explicit DataClient(const QString& ipToConnect, quint32 port, QObject* parent = nullptr);

    // Send functions
    void sendData(char *data, quint32 dataSize);

   public slots:
    void slotConnected();
    void slotReadyToRead();
    void slotError(QAbstractSocket::SocketError err);

   signals:
    // Receive signal
    void signalDataWasReceived(QByteArray data);

   private:
    QByteArray data_;
    quint32 blockSize_ = 0;
    QTcpSocket socket_;
    QDataStream dataStream_;
};
