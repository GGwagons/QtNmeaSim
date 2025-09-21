# QtNmeaSim

In space industry GNSS is used for obtaining precise location and time. This is most commonly done
using GNSS module which communicates with main computer via NMEA messages over UART. For
PC SW the NMEA messages are usually passed through serial port.

Implement Qt/C++ based solution with 3 topics:
1. Write a GNSS NMEA messages simulator (since data feed over serial port is not available)
a. Uses all lines from nmea.txt as simulated data, including bootup info

b. Simulated data can be either hardcoded inside code or read from .txt file

c. Every second transmits a burst of NMEA lines, beginning with $GPGGA and ending
with $GPZDA

d. Transmission of NMEA message should be done by emitting a signal “void
transmitNmea(QString nmea)”

2. NMEA parser:

a. Uses slots to retrieve data from transmitNmea(QString nmea) signal

b. Discards boot info

c. Verifies checksum of NMEA messages of interest

d. Parses fix mode, position and time (when available)

e. Each line is parsed or discarded as soon as it is received (real time parsing)

3. Consumer:
a. Notifies user via qDebug() when entering or leaving 3D fix mode

b. When in 3D fix mode, it prints position and time

c. Notifications are done in real time

Deliverables:
- Buildable Windows Qt project
