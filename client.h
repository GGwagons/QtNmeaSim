#pragma once
#include <QObject>
#include <QDateTime>


class Client : public QObject {
    Q_OBJECT
    public:
        explicit Client(QObject *parent = nullptr);


    public slots:
        void onFixModeChanged(int fixType);
        void onPositionTimeAvailable(const QDateTime &utc, double lat, double lon);


    private:
        bool m_in3D;
};
