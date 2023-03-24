/*
双臂机器人驱动代码

*/
//PCA9685 头文件
#include "robot_PCF8591.h"
#include "robot_PCA9685.h"
#include "robot_serial.h"
#include "robot_drv_eng.h"
#include "roubot_two_new_struct.h"
#include <Arduino.h>

int ctl_cmd[ENGINES_NUM] = {0};

void setup() {
  int ctl_init_cmd[ENGINES_NUM] = {0};
  int ret;

  //串口通信初始化
  Serial.begin(9600);
  robot_PCA9685_begin(50);
  robot_PCF8591_begin();

  robot_serial_init();
  engine_drv_init(5);

  ret = drv_engines(ctl_init_cmd, ENGINES_NUM);
  if(ret != 0)
  {
    Serial.println("[ERROR] setup: init postion engines error.");
  }

  Serial.println("[debug] setup: init finish.");
}

void loop() {

  int ret = 0;
  int vol = 0;

  // 接收并解析串口命令执行函数
  ret = robot_serial_execute(ctl_cmd, ENGINES_NUM);
  if(ret < 0)
  {
    Serial.print("[ERROR] main: robot_serial_execute error.ret : ");
    Serial.println(ret);
  }

  // 按串口命令驱动舵机函数
  ret = drv_engines(ctl_cmd, ENGINES_NUM);
  if(ret != 0)
  {
    Serial.print("[main] ERROR: drive engines error.ret : ");
    Serial.println(ret);
  }

  delay(5);
}
