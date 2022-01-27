#include <Arduino.h>
#include <RtcDS3231.h>
#include <Helios.h>
#include "consts.h"
#include "helpers.h"
#include "date.h"
#include "motor.h"


Helios helios;

Position getSunPostion(Coordinates& co, RtcDateTime& time) {
    helios.calcSunPos(time.Year(), time.Month(), time.Day(), time.Hour()-(TIMEZONE_OFFSET), time.Minute(), time.Second(), co.lng, co.lat);
    Position pos;
    pos.azimuth = helios.dAzimuth;
    pos.elevation = helios.dElevation;
    return pos;
}

void print_data(const int activeMode, const RtcDateTime& dt, Position& sun, Position& plant) {
    printDateTime(dt);
    Serial.print("\t");
    switch(activeMode) {
        case MODE_MANUAL:
            Serial.print("manual");
            break;
        case MODE_MORNING:
            Serial.print("morning");
            break;
        case MODE_NIGHT:
            Serial.print("night");
            break;
        case MODE_NORMAL:
            Serial.print("normal");
            break;
        case MODE_STORM:
            Serial.print("storm");
            break;
    }
#ifdef AZIMUTH_ENABLED
    Serial.print("\t| ");
    Serial.print(sun.azimuth, 0);
    Serial.print("'\t");
    Serial.print(plant.azimuth, 0);
    Serial.print("'\t");
    Serial.print(plant.azimuthRaw);
    Serial.print("\t| ");
#endif

#ifdef ELEVATION_ENABLED
    Serial.print(sun.elevation, 0);
    Serial.print("'\t");
    Serial.print(plant.elevation, 0);
    Serial.print("'\t");
    Serial.print(plant.elevationRaw);
    Serial.print("\t| ");
#endif

    if(get_azimuth_movement() == DIRECTION_RIGHT) Serial.print("->right");
    else if(get_azimuth_movement() == DIRECTION_LEFT) Serial.print("->left");
    else if(get_elevation_movement() == DIRECTION_UP) Serial.print("->up");
    else if(get_elevation_movement() == DIRECTION_DOWN) Serial.print("->down");
    Serial.println();
}