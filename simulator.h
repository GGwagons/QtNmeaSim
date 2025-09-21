#pragma once
#include <QObject>
#include <QString>
#include <QStringList>


class QTimer;

class Simulator : public QObject {
    Q_OBJECT
    public:
        explicit Simulator(const QString &filePath = QString(), QObject *parent = nullptr);
        void start();


    signals:
        void transmitNmea(QString nmea);


    private slots:
        void onTick();


    private:
        QStringList m_lines;
        int m_index;
        QTimer *m_timer;
        void loadFromFile(const QString &filePath);
};
