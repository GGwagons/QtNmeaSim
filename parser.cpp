#include "parser.h"
#include <QDebug>


Parser::Parser(QObject *parent) : QObject(parent), m_currentFixType(0), m_lastLat(0.0), m_lastLon(0.0) {}

bool Parser::verifyChecksum(const QString &line) {
    if (!line.startsWith('$')) {
        return false;
    }

    int asterisk = line.indexOf('*');
    if (asterisk < 0 || asterisk + 3 > line.length()) {
        return false;
    }

    QByteArray payload = line.mid(1, asterisk - 1).toLatin1();

    uint8_t cs = 0;
    for (int i = 0; i < payload.size(); ++i) {
        cs ^= static_cast<uint8_t>(payload[i]);
    }

    QString csText = line.mid(asterisk + 1, 2).toUpper();
    bool ok;
    uint8_t csHex = static_cast<uint8_t>(csText.toUInt(&ok, 16));
    if (!ok) {
        return false;
    }
    return cs == csHex;
}

double Parser::parseNmeaLat(const QString &field, const QString &dir) {
    if (field.isEmpty()) {
        return 0.0;
    }
    double v = field.toDouble();
    int deg = static_cast<int>(v / 100.0);
    double min = v - deg*100.0;
    double dec = deg + (min / 60.0);
    if (dir == "S") {
        dec = -dec;
    }
    return dec;
}


double Parser::parseNmeaLon(const QString &field, const QString &dir) {
    if (field.isEmpty()) {
        return 0.0;
    }
    double v = field.toDouble();
    int deg = static_cast<int>(v / 100.0);
    double min = v - deg*100.0;
    double dec = deg + (min / 60.0);
    if (dir == "W") {
        dec = -dec;
    }
    return dec;
}

void Parser::gpgga(QStringList &f) {
    double lat = parseNmeaLat(f[2], f[3]);
    double lon = parseNmeaLon(f[4], f[5]);
    int fixQuality = f[6].toInt();

    if (fixQuality > 0 && m_currentFixType == 3) {
        QDateTime utc;
        if (m_lastTimeUtc.isValid()) {
            utc = m_lastTimeUtc;
            if(f[1].length() >= 6) {
                utc.setTime(QTime(f[1].mid(0, 2).toInt(),f[1].mid(2, 2).toInt(), f[1].mid(4, 2).toInt()));
            }
        }
        else {
            QTime time(f[1].mid(0, 2).toInt(),f[1].mid(2, 2).toInt(), f[1].mid(4, 2).toInt());
            utc = QDateTime(QDate::currentDate(), time, Qt::UTC);
        }
        emit positionTimeAvailable(utc, lat, lon);
    }
}

void Parser::gpgsa(QStringList &f) {
    if (f.size() < 3) {
        return;
    }
    int fixMode = f[2].toInt();
    if (fixMode != m_currentFixType) {
        m_currentFixType = fixMode;
        emit fixModeChanged(fixMode);
    }
}

void Parser::gpzda(QStringList &f) {
    if (f.size() < 5) {
        return;
    }

    QString utcTime = f[1];
    QStringView view(utcTime);

    if (utcTime.length() < 6) {
        return;
    }

    QDate date(f[4].toInt(), f[3].toInt(), f[2].toInt());
    QTime time(view.mid(0, 2).toInt(),view.mid(2, 2).toInt(), view.mid(4, 2).toInt());
    QDateTime utc(date, time, Qt::UTC);

    m_lastTimeUtc = utc;
}

void Parser::onReceiveNmea(const QString &rawLine) {
    QString line = rawLine.trimmed();
    if (!line.startsWith('$')) {
        return;
    }

    if (!verifyChecksum(line)) {
        qWarning() << "Bad checksum:" << line;
        return;
    }

    QStringList f = line.split(',');
    QString msgId = f[0].mid(1);
    //qDebug() << msgId;

    if (msgId == "GPGGA") {
        gpgga(f);
    }
    else if (msgId == "GPGSA") {
        gpgsa(f);
    }
    else if (msgId == "GPZDA") {
        gpzda(f);
    }
}

