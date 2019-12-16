#pragma once

#include <QByteArray>
#include <QDataStream>
#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>
#include <iostream>

/**
 * @brief DataServer is a QTcpSocket wrapper that allow you to accept connection from DataClient
 */
class DataServer : public QObject {
    Q_OBJECT

   public:
    explicit DataServer(quint32 port, QObject* parent = nullptr);

    // Send functions
    void sendData(char* data, quint32 size);

   public slots:
    void slotNewConnection();
    void slotReadyToRead();
    void slotClientDisconnected();

   signals:
    void signalDataWasReceived(QByteArray data);

   private:
    QByteArray data_;
    QTcpServer* server_ = nullptr;
    QTcpSocket* socket_ = nullptr;
    QDataStream dataStream_;
    quint32 blockSize_ = 0;
};
