#include "objectinfo.h"

/**
 * @brief operator << - overloading operator to serialize ObjectInfo in QDataStream
 * @param serializator - serialize objInfo in serializator
 * @param objInfo - data to serialize
 * @return
 */
QDataStream& operator<<(QDataStream& serializator, const ObjectInfo& objInfo) {
    serializator << objInfo.classId;
    serializator << objInfo.confidence;
    serializator << objInfo.rect;
    serializator << objInfo.className;

    return serializator;
}

/**
 * @brief operator >> - overloading operator to deserialize ObjectInfo from QDataStream
 * @param serializator - deserialize objInfo from serializator
 * @param objInfo - object where data will be deserialized
 * @return
 */
QDataStream& operator>>(QDataStream& serializator, ObjectInfo& objInfo) {
    serializator >> objInfo.classId;
    serializator >> objInfo.confidence;
    serializator >> objInfo.rect;
    serializator >> objInfo.className;

    return serializator;
}
