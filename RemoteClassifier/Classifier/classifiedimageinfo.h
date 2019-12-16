#pragma once
#include <QDataStream>
#include <QVector>
#include <QtGlobal>
#include "objectinfo.h"

/**
 * @brief ClassifiedImageInfo - includes image id and vector of ObjectInfo's that describe all
 * objects from classified file
 */
struct ClassifiedImageInfo {
    quint32 imageId = 0;
    QVector<ObjectInfo> objects;

    QByteArray serialize();
    void fillFromQByteArray(QByteArray serialized);
};
