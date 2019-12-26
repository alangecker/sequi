#include "sensor.h"
#include "helpers.h"
#include "consts.h"
#include <Arduino.h>
#include <math.h>

void sensor_setup(int I2C_Address) {
    analogReference(EXTERNAL);
}

#ifdef AZIMUTH_ENABLED
uint32_t getAzimuthSensorValue() {
    uint32_t sum = 0;
    int16_t mes;
    for(int i=0;i<16;i++) {
        mes = analogRead(PIN_AZIMUTH_SENSOR);
        sum += mes;
        delay(10);
    }
    return sum/16;
}
#endif

#ifdef ELEVATION_ENABLED
uint32_t getElevationSensorValue() {
    uint32_t sum = 0;
    int16_t mes;
    for(int i=0;i<16;i++) {
        mes = analogRead(PIN_ELEVATION_SENSOR);
        sum += mes;
        delay(10);
    }
    return sum/16;
}
#endif

Position get_plant_postion() {
    Position res;

#ifdef AZIMUTH_ENABLED
    res.azimuthRaw = getAzimuthSensorValue();
    res.azimuth = (double)res.azimuthRaw*(double)AZIMUTH_SCALE + (double)AZIMUTH_OFFSET;
#endif

#ifdef ELEVATION_ENABLED
    res.elevationRaw = getElevationSensorValue();
    res.elevation = (double)res.elevationRaw*(double)ELEVATION_SCALE + (double)ELEVATION_OFFSET;
#endif
    return res;
}


