#include "remoteclassifierserver.h"

/**
 * @brief RemoteClassifierServer::initWorkers - creates ClassifyWorkerThreads that will classify in
 * several threads
 * @param numOfThreads - number of threads you want to use
 */
void RemoteClassifierServer::initWorkers(int numOfThreads) {
    for (int i = 0; i < numOfThreads; ++i) {
        workers_.emplace_back(imageInfoQueue_, queueMutex_);

        // Signal when image added in imageInfoQueue_
        connect(this, &RemoteClassifierServer::signalAddedInQueue, &workers_.back(),
                &ClassifyWorkerThread::slotClassify, Qt::ConnectionType::QueuedConnection);

        // Signal when image was classified
        connect(&workers_.back(), &ClassifyWorkerThread::signalClassified, this,
                &RemoteClassifierServer::slotClassified);
    }
}

/**
 * @brief RemoteClassifierServer::RemoteClassifierServer - constructor
 * @param port - server port
 */
RemoteClassifierServer::RemoteClassifierServer(quint32 port)
    : server_(port), classifier_("yolov3.cfg", "yolov3.weights", "coco.names") {
    connect(&server_, &DataServer::signalDataWasReceived, this,
            &RemoteClassifierServer::slotDataWasReceived);
    initWorkers(QThread::idealThreadCount());
}

/**
 * @brief RemoteClassifierServer::slotDataWasReceived - to process received ImageInfo from client
 * @param imageInfoBytes - received ImageInfo as QByteArray
 */
void RemoteClassifierServer::slotDataWasReceived(QByteArray imageInfoBytes) {
    ImageInfo imgInfo;
    imgInfo.fillFromQByteArray(imageInfoBytes);
    {
        std::lock_guard lock(queueMutex_);
        imageInfoQueue_.push(imgInfo);
    }
    emit signalAddedInQueue();
}

/**
 * @brief RemoteClassifierServer::slotClassified - when image classified
 * @param classifiedImgInfoBytes - ClassifiedImageInfo as QByteArray
 */
void RemoteClassifierServer::slotClassified(QByteArray classifiedImgInfoBytes) {
    server_.sendData(classifiedImgInfoBytes.data(), classifiedImgInfoBytes.size());
}
