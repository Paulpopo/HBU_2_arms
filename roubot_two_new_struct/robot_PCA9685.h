#ifndef __ROBOT_PCA8591_H__
#define __ROBOT_PCA8591_H__

#include <Arduino.h>

void robot_PCA9685_begin(int32_t time);
void engine_drv_init(int engine_num);
int Servo_out(uint16_t pca,uint16_t mode,uint16_t num, int32_t angle, int vol);

#endif
