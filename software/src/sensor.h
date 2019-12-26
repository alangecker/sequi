#include <Arduino.h>
#include "helpers.h"

void sensor_setup(int I2C_Address);
uint32_t getAzimuthSensorValue();

Position get_plant_postion();