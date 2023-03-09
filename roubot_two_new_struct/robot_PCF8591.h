#ifndef __ROBOT_PCF8591_H__
#define __ROBOT_PCF8591_H__

#include <Arduino.h>
#include "PCF8591.h"

void robot_PCF8591_begin();
int current_GET(int32_t pcf,int32_t mode,int32_t value);

#endif
