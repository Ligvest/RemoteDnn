#include "classifiedimageinfo.h"

/**
 * @brief ClassifiedImageInfo::serialize - converts object to QByteArray
 * @return
 */
QByteArray ClassifiedImageInfo::serialize() {
    QByteArray serialized;
    QDataStream serializator(&serialized, QIODevice::WriteOnly);
    serializator << imageId;
    serializator << objects;

    return serialized;
}

/**
 * @brief ClassifiedImageInfo::fillFromQByteArray - converts QByteArray to object
 * @param serialized - object as QByteArray
 */
void ClassifiedImageInfo::fillFromQByteArray(QByteArray serialized) {
    QDataStream serializator(&serialized, QIODevice::ReadOnly);
    serializator >> imageId;
    serializator >> objects;
}
