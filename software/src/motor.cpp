#include <Arduino.h>
#include "consts.h"
#include "motor.h"

int azimuth_movement = 0;
int elevation_movement = 0;

#define MOTOR_OFF LOW
#define MOTOR_ON HIGH


void motor_setup() {

#ifdef AZIMUTH_ENABLED
  digitalWrite(PIN_AZIMUTH_RIGHT, MOTOR_OFF);
  digitalWrite(PIN_AZIMUTH_LEFT, MOTOR_OFF);
  pinMode(PIN_AZIMUTH_RIGHT, OUTPUT);
  pinMode(PIN_AZIMUTH_LEFT, OUTPUT);
#endif

#ifdef ELEVATION_ENABLED
  digitalWrite(PIN_ELEVATION_UP, MOTOR_OFF);
  digitalWrite(PIN_ELEVATION_DOWN, MOTOR_OFF);
  pinMode(PIN_ELEVATION_UP, OUTPUT);
  pinMode(PIN_ELEVATION_DOWN, OUTPUT);
#endif

}


int get_azimuth_movement() {
    return azimuth_movement;
}
#ifdef AZIMUTH_ENABLED
void azimuth_start(int direction) {
    if(azimuth_movement == direction) return;
    azimuth_movement = direction;
    if(azimuth_movement == DIRECTION_LEFT) {
        digitalWrite(PIN_AZIMUTH_RIGHT, MOTOR_OFF);
        digitalWrite(PIN_AZIMUTH_LEFT, MOTOR_ON);
    } else {
        digitalWrite(PIN_AZIMUTH_RIGHT, MOTOR_ON);
        digitalWrite(PIN_AZIMUTH_LEFT, MOTOR_OFF);
    }
}
void azimuth_stop() {
    azimuth_movement = 0;
    digitalWrite(PIN_AZIMUTH_RIGHT, MOTOR_OFF);
    digitalWrite(PIN_AZIMUTH_LEFT, MOTOR_OFF);
}
#endif

int get_elevation_movement() {
    return elevation_movement;
}

#ifdef ELEVATION_ENABLED

void elevation_start(int direction) {
    if(elevation_movement == direction) return;
    elevation_movement = direction;
    if(elevation_movement == DIRECTION_UP) {
        digitalWrite(PIN_ELEVATION_DOWN, MOTOR_OFF);
        digitalWrite(PIN_ELEVATION_UP, MOTOR_ON);
    } else {
        digitalWrite(PIN_ELEVATION_DOWN, MOTOR_ON);
        digitalWrite(PIN_ELEVATION_UP, MOTOR_OFF);
    }
}
void elevation_stop() {
    elevation_movement = 0;
    digitalWrite(PIN_ELEVATION_DOWN, MOTOR_OFF);
    digitalWrite(PIN_ELEVATION_UP, MOTOR_OFF);
}
#endif