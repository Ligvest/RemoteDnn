#include "dataserver.h"

/**
 * @brief DataServer::DataServer - constructor
 * @param port - server port
 * @param parent - param inherited from QObject
 */
DataServer::DataServer(quint32 port, QObject* parent) : QObject(parent) {
    server_ = new QTcpServer(this);

    connect(server_, &QTcpServer::newConnection, this, &DataServer::slotNewConnection);

    if (!server_->listen(QHostAddress::Any, 6000)) {
        qDebug() << "Server is not started";
    } else {
        qDebug() << "Server is started";
    }
}

/**
 * @brief DataServer::slotNewConnection - to process new connection to the server
 */
void DataServer::slotNewConnection() {
    std::cerr << "Established new connection" << std::endl;
    socket_ = server_->nextPendingConnection();
    connect(socket_, &QTcpSocket::readyRead, this, &DataServer::slotReadyToRead);
    connect(socket_, &QTcpSocket::disconnected, this, &DataServer::slotClientDisconnected);
    dataStream_.setDevice(socket_);
}

/**
 * @brief DataServer::slotReadyToRead - slot to process received data.
 * Reads data in QByteArray and passes it in signal
 */
void DataServer::slotReadyToRead() {
    while (true) {
        if (blockSize_ == 0) {
            if (socket_->bytesAvailable() < sizeof(quint32)) {
                break;
            }
            dataStream_ >> blockSize_;
        }

        if (socket_->bytesAvailable() < blockSize_) {
            break;
        }

        dataStream_ >> data_;

        emit signalDataWasReceived(data_);

        data_.clear();
        blockSize_ = 0;
    }
}

/**
 * @brief DataServer::slotClientDisconnected - to process client disconnection
 */
void DataServer::slotClientDisconnected() { socket_->close(); }

/**
 * @brief DataServer::sendData - send data to client
 * @param data - array with data
 * @param dataSize - size of data in bytes
 */
void DataServer::sendData(char* data, quint32 dataSize) {
    QByteArray serializedData;
    QDataStream serializator(&serializedData, QIODevice::ReadWrite);

    // Reserver quint32 size, write data and then write real size in reserved memory
    serializator << quint32(0);
    serializator.writeBytes(data, dataSize);
    serializator.device()->seek(0);
    serializator << (quint32)(serializedData.size() - sizeof(quint32));

    auto written = socket_->write(serializedData, serializedData.size());
    if (written < 0) {
        std::cerr << "Error when writing into socket" << std::endl;
    }
}
