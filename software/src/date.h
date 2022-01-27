// #include "Arduino.h"
#include <SoftwareWire.h> 
#include <RtcDS3231.h>

#ifndef DATE_H
#define DATE_H

// #define J2000 2451545
// void printDateTime(const RtcDateTime& dt);
void date_setup();

void date_update(uint32_t secondsFrom2000);

void printDateTime(const RtcDateTime& dt);
RtcDateTime getDate();

#endif