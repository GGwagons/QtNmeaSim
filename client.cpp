#include "client.h"
#include <QDebug>


Client::Client(QObject *parent) : QObject(parent), m_in3D(false) {}

void Client::onFixModeChanged(int fixType) {
    bool now3D = (fixType == 3);
    if (now3D && !m_in3D) {
        m_in3D = true;
        qDebug() << "ENTERED 3D FIX MODE";
    }
    else if (!now3D && m_in3D) {
        m_in3D = false;
        qDebug() << "LEFT 3D FIX MODE";
    }
}

void Client::onPositionTimeAvailable(const QDateTime &utc, double lat, double lon) {
    qDebug().nospace() << "POS: lat=" << lat << ", lon=" << lon << ", UTC=" << utc.toString(Qt::ISODate);
}
