#ifndef NMEA_UTILS_H
#define NMEA_UTILS_H

#include <string>

// Function to calculate the CRC for an NMEA string
std::string getCRC(const std::string& nmea);

// Function to set the NMEA short LXWP0 message
std::string setNmeaShortLXWP0(float varioAlt, float climbRate);

#endif // NMEA_UTILS_H
