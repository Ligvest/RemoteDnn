Server must do:
RemoteClassifierServer server_(port);

Client must do:
RemoteClassifier client_(ipaddress, port);

connect(&client_, &RemoteClassifier::signalImageWasClassified, this, &Window::slotImageWasClassified);

void Window::slotImageWasClassified(ClassifiedImageInfo classifiedImgInfo) {
    //process ClassifiedImageInfo
}

client_.send({int imageId, cv::Mat image });
