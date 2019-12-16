#pragma once
#include <QDataStream>
#include <QRect>
#include <QString>
#include <QtGlobal>

/**
 * @brief The ObjectInfo struct - info about one classified object
 */
struct ObjectInfo {
    quint32 classId;
    qreal confidence;
    QRect rect;
    QString className;
};

QDataStream& operator<<(QDataStream& serializator, const ObjectInfo& objInfo);
QDataStream& operator>>(QDataStream& serializator, ObjectInfo& objInfo);
