#include <iostream>
#include <string>
#include <cassert>
#include <cstdio>
#include "nmea.hpp"

std::string getCRC(const std::string& nmea) {
    unsigned char XOR = 0;
    assert(nmea.front() == '$');
    assert(nmea.back() == '*');
    
    for (size_t i = 1; i < nmea.size() - 1; ++i) {
        XOR ^= nmea[i];
    }

    char crc[3]; // Two characters for the hex value + one for the null terminator
    snprintf(crc, sizeof(crc), "%x", XOR);

    return std::string(crc);
}

// Function to set the NMEA short LXWP0
std::string setNmeaShortLXWP0(float varioAlt, float climbRate) {
    char nmea[100]; // Adjust size as needed
    snprintf(nmea, sizeof(nmea), "$LXWP0,N,,%.2f,%.2f,,,,,,,,,*", varioAlt, climbRate);
    
    std::string nmeaStr(nmea);
    std::string CRC = getCRC(nmeaStr);
    nmeaStr += CRC + "\r\n";
    
    return nmeaStr;
}