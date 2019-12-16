#pragma once

#include <QObject>
#include <QThread>
#include <mutex>
#include <queue>
#include "Classifier/classifier.h"
#include "imageinfo.h"

/**
 * @brief ClassifyWorkerThread - wrapper over Classifier that creates new thread for each instance
 * of the class
 */
class ClassifyWorkerThread : public QObject {
    Q_OBJECT

   public:
    ClassifyWorkerThread(std::queue<ImageInfo>& imageInfoQueue, std::mutex& queueMutex,
                         QObject* parent = nullptr);

    ~ClassifyWorkerThread();

   public slots:
    void slotClassify();

   signals:
    void signalClassified(QByteArray);

   private:
    Classifier classifier_;
    QThread thread_;
    std::queue<ImageInfo>& imageInfoQueue_;
    std::mutex& queueMutex_;
};
