#ifndef WINDOW_H
#define WINDOW_H

#include <QDir>
#include <QImage>
#include <QList>
#include <QWidget>
#include <experimental/filesystem>
#include <fstream>
#include <iostream>
#include <opencv2/dnn.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include "RemoteClassifier/remoteclassifier.h"
#include "RemoteClassifier/remoteclassifierserver.h"
#include "profile_advanced.h"

namespace Ui {
class Window;
}

class Window : public QWidget {
    Q_OBJECT

   public:
    explicit Window(QWidget* parent = nullptr);
    ~Window();

    void drawPred(QString classNameInp, float conf, QRect rectInp, cv::Mat& frame);
    void showImageWithRects(cv::Mat image, ClassifiedImageInfo* classifiedImgInfo = nullptr);

    // debug
    void writeImg(cv::Mat image, ClassifiedImageInfo* classifiedImgInfo);

   private slots:
    void on_pushButtonNextImage_clicked();
    void slotImageWasClassified(ClassifiedImageInfo classifiedImgInfo);

   private:
    std::string prevFileName();
    std::string nextFileName();

   private:
    Ui::Window* ui;
    RemoteClassifierServer server_;
    RemoteClassifier client_;
    std::string dirPath_;
    cv::VideoCapture capture_;
    cv::Mat image_;
    QStringList fileNames_;
    QStringList::iterator fileNameIt_;

    int imagesToSend_ = 0;
    steady_clock::time_point start_;
    int imagesSent_ = 0;
    int imagesReceived_ = 0;
};

#endif  // WINDOW_H
