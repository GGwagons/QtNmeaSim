#include "simulator.h"
#include <QFile>
#include <QTextStream>
#include <QTimer>
#include <QDebug>


Simulator::Simulator(const QString &filePath, QObject *parent) : QObject(parent), m_index(0), m_timer(new QTimer(this)) {
    if (!filePath.isEmpty()) {
        loadFromFile(filePath);
    }
    connect(m_timer, &QTimer::timeout, this, &Simulator::onTick);
}


void Simulator::loadFromFile(const QString &filePath) {
    QFile f(filePath);
    if (!f.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qWarning() << "Could not open" << filePath << "— simulator will use embedded defaults.";
        qWarning() << "in main-> Simulator simulator(\"[specify filePath]\")";
        exit(1);
    }
    QTextStream in(&f);
    while (!in.atEnd()) {
        QString line = in.readLine();
        if (!line.isEmpty()) {
            m_lines.append(line.trimmed());
        }
    }
}


void Simulator::start() {
    if (m_lines.isEmpty()) {
        qWarning() << "No NMEA lines loaded; cannot start simulator.";
        return;
    }
    m_timer->start(1000);
}

void Simulator::onTick() {
    // qDebug() << "Tick fired";
    const int total = m_lines.size();
    if (total == 0) return;

    int start = -1;
    for (int i = 0; i < total; ++i) {
        int idx = (m_index + i) % total;
        if (m_lines[idx].startsWith("$GPGGA")) { start = idx; break; }
    }

    if (start == -1) {
        emit transmitNmea(m_lines[m_index]);
        m_index = (m_index + 1) % total;
        return;
    }

    int idx = start;
    while (true) {
        emit transmitNmea(m_lines[idx]);
        if (m_lines[idx].startsWith("$GPZDA")) {
            break;
        }
        idx = (idx + 1) % total;
        if (idx == start) {
            break;
        }
    }
    m_index = (idx + 1) % total;
}

