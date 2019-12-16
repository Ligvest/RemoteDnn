#include "imageinfo.h"

/**
 * @brief ImageInfo::serialize - convert ImageInfo to QByteArray
 * @return - ImageInfo as QByteArray
 */
QByteArray ImageInfo::serialize() {
    QByteArray serialized;
    QDataStream serializator(&serialized, QIODevice::WriteOnly);
    serializator << imageId;
    serializator << serializeImg(image);

    return serialized;
}

/**
 * @brief ImageInfo::fillFromQByteArray - convert QByteArray to ImageInfo
 * @param serialized - ImageInfo as QByteArray
 */
void ImageInfo::fillFromQByteArray(QByteArray serialized) {
    QDataStream serializator(&serialized, QIODevice::ReadOnly);
    serializator >> imageId;

    QByteArray serializedImage;
    serializator >> serializedImage;
    image = deserializeImg(serializedImage);
}

/**
 * @brief serializeImg - convert cv::Mat to QByteArray
 * @param img - cv::Mat to convert to QByteArray
 * @return - cv::Mat as QByteArray
 */
QByteArray serializeImg(cv::Mat img) {
    QByteArray serializedData;
    QDataStream serialized(&serializedData, QIODevice::ReadWrite);
    serialized << static_cast<quint32>(img.rows);
    serialized << static_cast<quint32>(img.cols);
    serialized << static_cast<quint32>(img.type());

    QByteArray data(reinterpret_cast<char*>(img.data), img.total() * img.elemSize());
    serialized << data;

    return serializedData;
}

/**
 * @brief deserializeImg - convert QByteArray to cv::Mat
 * @param serializedData - cv::Mat as QByteArray
 * @return - cv::Mat obtained from serilalizedData
 */
cv::Mat deserializeImg(QByteArray serializedData) {
    QDataStream serialized(&serializedData, QIODevice::ReadWrite);
    quint32 rows;
    quint32 cols;
    quint32 type;
    serialized >> rows;
    serialized >> cols;
    serialized >> type;

    QByteArray data;
    serialized >> data;

    cv::Mat resImg(rows, cols, type);
    memcpy(resImg.data, data.data(), data.size());

    return resImg;
}
