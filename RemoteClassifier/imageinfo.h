#pragma once

#include <QByteArray>
#include <QDataStream>
#include <QtGlobal>
#include <opencv2/core/mat.hpp>

/**
 * @brief ImageInfo - includes cv::Mat image and some addition information for sending to remotely
 * classify
 */
struct ImageInfo {
    quint32 imageId;
    cv::Mat image;

    QByteArray serialize();
    void fillFromQByteArray(QByteArray serialized);
};

QByteArray serializeImg(cv::Mat img);
cv::Mat deserializeImg(QByteArray serializedData);
