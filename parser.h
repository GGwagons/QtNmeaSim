#pragma once
#include <QObject>
#include <QString>
#include <QDateTime>


class Parser : public QObject {
    Q_OBJECT
    public:
        explicit Parser(QObject *parent = nullptr);


    public slots:
        void onReceiveNmea(const QString &line);


    signals:
        void fixModeChanged(int fixType);
        void positionTimeAvailable(const QDateTime &utc, double latitudeDeg, double longitudeDeg);


    private:
        bool verifyChecksum(const QString &line);
        double parseNmeaLat(const QString &field, const QString &dir);
        double parseNmeaLon(const QString &field, const QString &dir);
        void gpgga(QStringList &f);
        void gpgsa(QStringList &f);
        void gpzda(QStringList &f);


        int m_currentFixType; // 0=none,1=2D,2=3D
        QDateTime m_lastTimeUtc;
        double m_lastLat;
        double m_lastLon;
};
