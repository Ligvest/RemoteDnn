#include "classifyworkerthread.h"

/**
 * @brief ClassifyWorkerThread::ClassifyWorkerThread - constructor
 * @param imageInfoQueue - ref to queue that contains ImageInfo's to classify
 * @param queueMutex - ref to mutes that blocks access to imageInfoQueue
 * @param parent - param inherited from QObject
 */
ClassifyWorkerThread::ClassifyWorkerThread(std::queue<ImageInfo>& imageInfoQueue,
                                           std::mutex& queueMutex, QObject* parent)
    : QObject(parent),
      classifier_("yolov3.cfg", "yolov3.weights", "coco.names"),
      imageInfoQueue_(imageInfoQueue),
      queueMutex_(queueMutex) {
    //
    moveToThread(&thread_);
    thread_.start();
}

/**
 * @brief ClassifyWorkerThread::~ClassifyWorkerThread - destructor
 */
ClassifyWorkerThread::~ClassifyWorkerThread() {
    thread_.quit();
    thread_.wait();
}

/**
 * @brief ClassifyWorkerThread::slotClassify - to classify ImageInfo from imageInfoQueue
 */
void ClassifyWorkerThread::slotClassify() {
    ImageInfo imgInfo;
    {
        std::lock_guard lock(queueMutex_);

        if (imageInfoQueue_.size() < 1) {
            return;
        }

        imgInfo = std::move(imageInfoQueue_.front());
        imageInfoQueue_.pop();
    }

    auto classified = classifier_.classify(imgInfo.image);
    ClassifiedImageInfo classifiedImgInfo =
        classifier_.parseClassified(classified, imgInfo.image.cols, imgInfo.image.rows);
    classifiedImgInfo.imageId = imgInfo.imageId;

    emit signalClassified(classifiedImgInfo.serialize());
}
