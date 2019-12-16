#pragma once
#include "Classifier/classifier.h"
#include "DataTransfer/dataclient.h"
#include "imageinfo.h"

/**
 * @brief RemoteClassifier - wrapper over DataClient and Classifier that let you connect to server
 * and remotely classify objects
 */
class RemoteClassifier : public QObject {
    Q_OBJECT
   public:
    RemoteClassifier(QString ipAddress, quint32 port);
    void classify(ImageInfo imgInfo);

   private slots:
    void slotDataWasReceived(QByteArray classifiedImageInfoBytes);

   signals:
    void signalImageWasClassified(ClassifiedImageInfo imgInfo);

   private:
    DataClient client_;
    ClassifiedImageInfo classifiedImgInfo_;
    bool isClassified_ = false;
};
