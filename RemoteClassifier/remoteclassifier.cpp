#include "remoteclassifier.h"

/**
 * @brief RemoteClassifier::RemoteClassifier - constructor
 * @param ipAddress - ip address of a server to connect
 * @param port - port of a server to connect
 */
RemoteClassifier::RemoteClassifier(QString ipAddress, quint32 port) : client_(ipAddress, port) {
    connect(&client_, &DataClient::signalDataWasReceived, this,
            &RemoteClassifier::slotDataWasReceived);
}

/**
 * @brief RemoteClassifier::classify - send ImageInfo to remotely classify the image in it
 * @param imgInfo - ImageInfo to classify
 */
void RemoteClassifier::classify(ImageInfo imgInfo) {
    QByteArray serializedImgInfo = imgInfo.serialize();
    client_.sendData(serializedImgInfo.data(), serializedImgInfo.size());
}

/**
 * @brief RemoteClassifier::slotDataWasReceived - to processed ImageInfo classified by a server
 * @param classifiedImageInfoBytes - ImageInfo as QByteArray
 */
void RemoteClassifier::slotDataWasReceived(QByteArray classifiedImageInfoBytes) {
    classifiedImgInfo_.fillFromQByteArray(classifiedImageInfoBytes);
    emit signalImageWasClassified(classifiedImgInfo_);
}
