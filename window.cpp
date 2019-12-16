#include "window.h"
#include "ui_window.h"

Window::Window(QWidget* parent)
    : QWidget(parent),
      ui(new Ui::Window),
      server_(6000),
      client_("127.0.0.1", 6000),
      dirPath_("../Resourses/Pictures/") {
    connect(&client_, &RemoteClassifier::signalImageWasClassified, this,
            &Window::slotImageWasClassified);

    QDir directory(dirPath_.c_str());
    fileNames_ = directory.entryList(QDir::Files);
    fileNameIt_ = fileNames_.begin();
    ui->setupUi(this);
}

Window::~Window() {
    capture_.release();
    delete ui;
}

void Window::on_pushButtonNextImage_clicked() {
    std::string filename = nextFileName();
    capture_.open(dirPath_ + filename);
    capture_ >> image_;

    // debug
    imagesToSend_ = 10;
    std::vector<cv::Mat> images;
    images.resize(imagesToSend_, image_);
    for (quint32 i = 0; i < images.size(); ++i) {
        client_.classify({i, images[i]});
        ++imagesSent_;
    }
    start_ = steady_clock::now();

    // client_.classify({1, image_});
    capture_.release();
}
void Window::showImageWithRects(cv::Mat image, ClassifiedImageInfo* classifiedImgInfo) {
    if (classifiedImgInfo != nullptr) {
        for (auto obj : classifiedImgInfo->objects) {
            drawPred(obj.className, obj.confidence, obj.rect, image);
        }
    }

    cv::cvtColor(image, image, cv::COLOR_BGR2RGB);
    QImage detectedImage(image.data, image.cols, image.rows, image.step, QImage::Format_RGB888);
    ui->labelImage->setPixmap(QPixmap::fromImage(detectedImage));
}

void Window::writeImg(cv::Mat image, ClassifiedImageInfo* classifiedImgInfo) {
    if (classifiedImgInfo != nullptr) {
        for (auto obj : classifiedImgInfo->objects) {
            drawPred(obj.className, obj.confidence, obj.rect, image);
        }
    }

    std::string outName = dirPath_ + std::to_string(imagesReceived_) + "outImg.png";
    cv::imwrite(outName, image);
}

void Window::slotImageWasClassified(ClassifiedImageInfo classifiedImgInfo) {
    //    showImageWithRects(image_, &classifiedImgInfo);

    ++imagesReceived_;
    std::cout << imagesReceived_ << std::endl;
    if (imagesReceived_ == imagesToSend_) {
        auto time = steady_clock::now() - start_;
        std::cout << "time passed: " << duration_cast<milliseconds>(time).count() << "ms"
                  << std::endl;
    }
}

std::string Window::nextFileName() {
    ++fileNameIt_;
    if (fileNameIt_ == fileNames_.end()) {
        fileNameIt_ = fileNames_.begin();
    }

    return fileNameIt_->toStdString();
}

std::string Window::prevFileName() {
    if (fileNameIt_ == fileNames_.begin()) {
        fileNameIt_ = fileNames_.end();
    }
    --fileNameIt_;

    return fileNameIt_->toStdString();
}

// Draw the predicted bounding box
void Window::drawPred(QString classNameInp, float conf, QRect rectInp, cv::Mat& frame) {
    cv::Rect rect(rectInp.left(), rectInp.top(), rectInp.width(), rectInp.height());
    std::string className(classNameInp.toStdString());
    // Draw a rectangle displaying the bounding box
    rectangle(frame, rect, cv::Scalar(255, 178, 50), 3);

    // Get the label for the class name and its confidence
    std::string label = cv::format("%.2f", conf);
    label = className + ":" + label;

    // Display the label at the top of the bounding box
    int baseLine;
    float textFontScale = 0.6f;
    cv::Scalar rectColor(255, 255, 255);
    cv::Scalar fontColor(0, 0, 0);
    cv::Size labelSize = getTextSize(label, cv::FONT_HERSHEY_SIMPLEX, textFontScale, 1, &baseLine);
    //    top = top + labelSize.height;
    rectangle(frame, cv::Point(rect.x, rect.y - labelSize.height),
              cv::Point(rect.x + labelSize.width, rect.y + baseLine), rectColor, cv::FILLED);
    putText(frame, label, cv::Point(rect.x, rect.y), cv::FONT_HERSHEY_SIMPLEX, textFontScale,
            fontColor, 1);
}
