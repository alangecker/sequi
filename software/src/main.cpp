#include <Arduino.h>
#include "date.h"
#include "helpers.h"
#include "sensor.h"
#include "motor.h"
#include "consts.h"
#include <RtcDS3231.h>



Coordinates plantLocation = {PLANT_LOCATION_LAT, PLANT_LOCATION_LNG};
Position plantPosition = {0, 0, 0, 0};
Position morningPosition = {0, 0, 0, 0};
Position stormPosition = {0, 0, 0, 0};
Position sunPosition = {0, 0, 0, 0};
int activeMode = MODE_NORMAL;

void setup() {
  Serial.begin(9600);
  date_setup();
  sensor_setup(0x4A);
  motor_setup();
  pinMode(PIN_STORM_SWITCH, INPUT);
  stormPosition.azimuth = STORM_AZIMUTH;
  stormPosition.elevation = STORM_ELEVATION;

  // todo: calculate morning position
  morningPosition.azimuth = 110; 
  morningPosition.elevation = ELEVATION_MIN;
}


void handleCommand(char cmd) {
      switch(cmd) {
        case 'z':
          while(true) {
            if(!Serial.available()) continue;
            uint32_t time = Serial.parseInt();
            Serial.println(time);
            date_update(time);
            return;
          }
          break;
#ifdef AZIMUTH_ENABLE
        case 'a':
          if(get_azimuth_movement() != DIRECTION_LEFT) {
            elevation_stop();
            azimuth_start(DIRECTION_LEFT);
          }
          break;
        case 'd':
          if(get_azimuth_movement() != DIRECTION_RIGHT) {
            elevation_stop();
            azimuth_start(DIRECTION_RIGHT);
          }
          break;
#endif
#ifdef ELEVATION_ENABLE
        case 'w':
          if(get_elevation_movement() != DIRECTION_UP) {
            azimuth_stop();
            elevation_start(DIRECTION_UP);
          }
          break;
        case 's':
          if(get_elevation_movement() != DIRECTION_DOWN) {
            azimuth_stop();
            elevation_start(DIRECTION_DOWN);
          }
          break;
#endif
      }
}


char rx_byte;
unsigned long lastManualCommand = __LONG_MAX__;
boolean inManualMode;
RtcDateTime now;
unsigned long loopCount = 0;

void loop() {
  now = getDate();

  sunPosition = getSunPostion(plantLocation, now);
  plantPosition = get_plant_postion();
  inManualMode = (lastManualCommand + 5l*60l*1000l) >= millis() && !(lastManualCommand > millis());
  // inManualMode = true;
  // print data sometimes
  if(loopCount++ % 10 == 0) {
    print_data(activeMode, now, sunPosition, plantPosition);
  }

  // check limits

#ifdef AZIMUTH_ENABLED
  if(!inManualMode && get_azimuth_movement() == DIRECTION_RIGHT && plantPosition.azimuth >= AZIMUTH_MAX) azimuth_stop();
  if(!inManualMode && get_azimuth_movement() == DIRECTION_LEFT && plantPosition.azimuth <= AZIMUTH_MIN) azimuth_stop();
#endif

#ifdef ELEVATION_ENABLED
  if(!inManualMode && get_elevation_movement() == DIRECTION_UP && plantPosition.elevation >= ELEVATION_MAX) elevation_stop();
  if(!inManualMode && get_elevation_movement() == DIRECTION_DOWN && plantPosition.elevation <= ELEVATION_MIN) elevation_stop();
#endif

  // already in manual mode?
  if(inManualMode) {
    activeMode = MODE_MANUAL;
    if (Serial.available() > 0) {
      rx_byte = Serial.read();       // get the character
      while(Serial.available() > 0) Serial.read(); // flush
      handleCommand(rx_byte);
      lastManualCommand = millis();
    }
    if(lastManualCommand + 600 < millis() && !(lastManualCommand > millis())) {
#ifdef AZIMUTH_ENABLED
      azimuth_stop();
#endif
#ifdef ELEVATION_ENABLED
      elevation_stop();
#endif
    }

  // got new command? switch to manual mode
  } else if (Serial.available() > 0) {
  
    lastManualCommand = millis();
    if(Serial.available() > 1) while(Serial.available() > 0) Serial.read(); // flush

  // normal loop
  } else {
    Position target;



    // is a storm blowing? 
    if(analogRead(PIN_STORM_SWITCH) > 512) {
      activeMode = MODE_STORM;
      target = stormPosition;

    // is sun already down?
    } else if(sunPosition.elevation < -10) {

      //after midnight? go to morning position
      if(now.Hour() < 12) {
        activeMode = MODE_MORNING;
        target = morningPosition;
      } else {
        activeMode = MODE_NIGHT;
        target = plantPosition; // = do nothing
      }
    } else {
      activeMode = MODE_NORMAL;
      target = sunPosition;
    }

#ifdef ELEVATION_ENABLED
    if(!get_azimuth_movement()) {
      if(plantPosition.elevation < target.elevation && get_elevation_movement() != DIRECTION_DOWN) {
        if(target.elevation-plantPosition.elevation > ELEVATION_TOLERANCE && target.elevation <= ELEVATION_MAX) {
          elevation_start(DIRECTION_UP);
        }
      } else if(plantPosition.elevation > target.elevation  && get_elevation_movement() != DIRECTION_UP) {
        if(plantPosition.elevation-target.elevation > ELEVATION_TOLERANCE && target.elevation >= ELEVATION_MIN) {
          elevation_start(DIRECTION_DOWN);
        }
      } else {
        elevation_stop();
      }
    }
#endif

#ifdef AZIMUTH_ENABLED
    if(!get_elevation_movement()) {
      if(plantPosition.azimuth < target.azimuth && get_azimuth_movement() != DIRECTION_LEFT) {
        if(target.azimuth-plantPosition.azimuth > AZIMUTH_TOLERANCE && target.azimuth <= AZIMUTH_MAX) {
          azimuth_start(DIRECTION_RIGHT);
        }
      } else if(plantPosition.azimuth > target.azimuth  && get_azimuth_movement() != DIRECTION_RIGHT) {
        if(plantPosition.azimuth-target.azimuth > AZIMUTH_TOLERANCE && target.azimuth >= AZIMUTH_MIN) {
          azimuth_start(DIRECTION_LEFT);
        }
      } else {
        azimuth_stop();
      }
    } 
#endif

    delay(100);
    loopCount = 10;
  }
}
