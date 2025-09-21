#include <QCoreApplication>
#include "simulator.h"
#include "parser.h"
#include "client.h"


int main(int argc, char *argv[]) {
    QCoreApplication a(argc, argv);
    Simulator simulator("nmea.txt"); // add filePath to make it run!
    Parser parser;
    Client client;

    // connect simulator -> parser
    QObject::connect(&simulator, &Simulator::transmitNmea, &parser, &Parser::onReceiveNmea);
    // connect parser -> client
    QObject::connect(&parser, &Parser::fixModeChanged, &client, &Client::onFixModeChanged);

    QObject::connect(&parser, &Parser::positionTimeAvailable, &client, &Client::onPositionTimeAvailable);

    simulator.start();
    qDebug() << "App started!";
    return a.exec();
}
