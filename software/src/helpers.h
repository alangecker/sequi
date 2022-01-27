#ifndef HELPERS_H
#define HELPERS_H
struct Coordinates {
    float lat;
    float lng;
};
struct Position {
    float azimuth;
    float elevation;
    uint32_t azimuthRaw;
    uint32_t elevationRaw;
};

#include <RtcDS3231.h>

Position getSunPostion(Coordinates& co, RtcDateTime& time);

void print_data(const int activeMode, const RtcDateTime& dt, Position& sun, Position& plant);
// void print_data(Position sun, Position plant);
#endif