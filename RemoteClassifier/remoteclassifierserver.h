#include <QThread>
#include <queue>
#include "Classifier/classifier.h"
#include "DataTransfer/dataserver.h"
#include "classifyworkerthread.h"
#include "imageinfo.h"

/**
 * @brief RemoteClassifierServer - wrapper over DataServer and Classifier that let you to create
 * server that will be classify images remotely in several threads
 */
class RemoteClassifierServer : public QObject {
    Q_OBJECT
   public:
    RemoteClassifierServer(quint32 port);

   private slots:
    void slotDataWasReceived(QByteArray imageInfoBytes);
    void slotClassified(QByteArray classifiedImgInfoBytes);

   signals:
    void signalAddedInQueue();

   private:
    void initWorkers(int numOfThreads);

   private:
    DataServer server_;
    Classifier classifier_;
    std::queue<ImageInfo> imageInfoQueue_;
    std::list<ClassifyWorkerThread> workers_;
    std::mutex queueMutex_;
};
