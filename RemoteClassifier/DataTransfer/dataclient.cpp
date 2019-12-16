#include "dataclient.h"

/**
 * @brief DataClient::DataClient - constructor
 * @param ipToConnect - server ip address to connect
 * @param port - server port to connect
 * @param parent - parameter inherited from QObject
 */
DataClient::DataClient(const QString& ipToConnect, quint32 port, QObject* parent)
    : QObject(parent), dataStream_(&socket_) {
    connect(&socket_, &QTcpSocket::readyRead, this, &DataClient::slotReadyToRead);
    connect(&socket_, &QTcpSocket::connected, this, &DataClient::slotConnected);
    connect(&socket_, QOverload<QAbstractSocket::SocketError>::of(&QAbstractSocket::error), this,
            &DataClient::slotError);
    socket_.connectToHost(ipToConnect, port, QIODevice::ReadWrite);
}

/**
 * @brief DataClient::slotConnected - when connection was successful
 */
void DataClient::slotConnected() {
    auto host = socket_.peerAddress();
    auto port = socket_.peerPort();
    std::cout << "You are connected to " << host.toString().toStdString() << ":" << port
              << std::endl;
}

/**
 * @brief DataClient::slotReadyToRead - slot to process received data.
 * Reads data in QByteArray and passes it in signal
 */
void DataClient::slotReadyToRead() {
    while (true) {
        if (blockSize_ == 0) {
            if (socket_.bytesAvailable() < sizeof(quint32)) {
                break;
            }
            dataStream_ >> blockSize_;
        }

        if (socket_.bytesAvailable() < blockSize_) {
            break;
        }

        dataStream_ >> data_;

        emit signalDataWasReceived(data_);

        data_.clear();
        blockSize_ = 0;
    }
}

/**
 * @brief DataClient::slotError - slot on default error signal from QTcpSocket
 * @param err
 */
void DataClient::slotError(QAbstractSocket::SocketError err) { std::cout << "error!" << std::endl; }

/**
 * @brief DataClient::sendData - send data to server
 * @param data - array with data
 * @param dataSize - size of data in bytes
 */
void DataClient::sendData(char* data, quint32 dataSize) {
    QByteArray serializedData;
    QDataStream serializator(&serializedData, QIODevice::ReadWrite);

    // Reserver quint32 size, write data and then write real size in reserved memory
    serializator << quint32(0);
    serializator.writeBytes(data, dataSize);
    serializator.device()->seek(0);
    serializator << (quint32)(serializedData.size() - sizeof(quint32));

    auto written = socket_.write(serializedData, serializedData.size());
    if (written < 0) {
        std::cerr << "Error when writing into socket" << std::endl;
    }
}
