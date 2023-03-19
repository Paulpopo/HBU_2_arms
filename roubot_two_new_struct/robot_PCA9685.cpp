//PCA9685 头文件
#include <stdlib.h>
#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>
#include "robot_PCA9685.h"
#include "robot_PCF8591.h"
#include "roubot_two_new_struct.h"

// PCA9685 IIC地址配置
Adafruit_PWMServoDriver pca1 = Adafruit_PWMServoDriver(0x41);
Adafruit_PWMServoDriver pca2 = Adafruit_PWMServoDriver(0x42);


//舵机输出pwm角度宏定义
#define SERVO_0  102
#define SERVO_45  187
#define SERVO_90  280
#define SERVO_135  373
#define SERVO_180  510

//电压
#define VOL 49
#define RUN 1
#define NOTRUN 0

/*
* 舵机转动的全局变量
* angel:当前角度 0~180
* target_angle:记录目标角度值，在堵转停止情况下，街道新的角度值，再开启转动用。
* *prt:均为动作执行标志，0 动作已执行；1 动作未执行
*/
struct robot_run_t
{
  float angle[ENGINES_NUM+1];
  int target_angle;

  int gtas_prt[ENGINES_NUM+1];
  unsigned char bs_prt;

};
robot_run_t robot_run;

/*
PCA9685初始化函数
PCA9685舵机控制需要50HZ更新频率，相当于20ms的周期
参数为设置PCA9685更新频率，即设置周期
控制舵机常用参数为50
*/
void robot_PCA9685_begin(int32_t time)
{
  pca1.begin();
  pca2.begin();
  delay(10);
  pca1.setPWMFreq(time);
  pca2.setPWMFreq(time);
}


void engine_drv_init(int engine_num)
{
    memset(robot_run.angle, 0, sizeof(robot_run.angle));

    robot_run.target_angle = 0;

    memset(robot_run.gtas_prt, 1, sizeof(robot_run.gtas_prt));

    robot_run.bs_prt = 0;
}


/*
* 功能：通过判断目标角度是否更新，和电压是否超标，而产生一个转动状态的标志
* 参数：angle 此次目标角度，用于与上次目标角度对比，若不同则更新为此次值，并产生一个转动标志 0~180
*       vol 当前电压值，若该值超过阈值，则产生一个停止转动信号
* 返回值：RUN/NOTRUN
*/
int change_run_status(int32_t angle, int vol)
{
  int run_status = RUN;

  //检查目标角度值是否更新，更新了就有转信号
  if(angle != robot_run.target_angle)
  {
    robot_run.target_angle = angle;
    run_status = RUN;
    Serial.println("[debug]change_run_status: target_angle changed.");
  }

  //检查电压值是否超标，超标了就有不转信号
  if(vol > VOL)
  {
    run_status = NOTRUN;

    Serial.println("[debug]change_run_status: voltage overload.");
    Serial.println(vol);
  }

  return run_status;
}

/*
* 功能：根据转动标志，选择芯片和通道逐度转动，达到目标角度自动停止
* 参数：pca_n 选择某个芯片 1/2
*       num 选择通道 0-11
*       angle 目标角度，同时目标角度和当前角度的大小关系控制正转和反转 0~180
*       runornot 转停标志 RUN/NOTRUN
* 返回值：0成功，其他失败
*/
int execute_run(uint16_t pca_n, uint16_t num, int32_t angle, int runornot)
{
  int pwmout;
  int num_prt;
  Adafruit_PWMServoDriver pca;
  char pca_s[8] = {0};
  char pca1_s[8] = "pac1";
  char pca2_s[8] = "pac2";

  // 因为两个芯片用的通道数相同，但是“是否打印”的标志位又不能相同，所以增加一个判断，
  // 若是pca1则num_prt=num，若是pca2则num_prt=num+10
  // 同时当前角度的顺序号也是与输入的顺序号不同的，所以也用num_prt
  if(1 == pca_n)
  {
    pca = pca1;
    strcpy(pca_s, pca1_s);

    num_prt = num;
  }
  else if (2 == pca_n)
  {
    pca = pca2;
    strcpy(pca_s, pca2_s);

    num_prt = num + 12;
  }
  else
  {
    Serial.println("[ERROR]execute_run: invalied pca.");
    return -1;
  }


  if((num > 11) || (num < 0))
  {
    Serial.print("[ERROR]execute_run: invalied num: ");
    Serial.println(num);
    return -1;
  }

  if((angle > 180) || (angle < -180))
  {
    Serial.println("[ERROR]execute_run: invalied angle.");
    return -1;
  }

  // Serial.print(". No. ");
  // Serial.print(num_prt);
  // Serial.print(" gtas_prt :");
  // Serial.println(robot_run.gtas_prt[num_prt]);


  // if((int)robot_run.angle[num] != angle)
  // {
    // robot_run.angle导致控制多个电机时当前角度被多个目标角度控制，从而导致电机之前冲突
    if(RUN == runornot)
    {
      pwmout=(robot_run.angle[num_prt]/180*2+0.5)*4096/20;
      pca.setPWM(num,0,pwmout);

      //根据当前角度值与目标角度值判断电机运动方向
      if((int)robot_run.angle[num_prt] > angle)
      {
        robot_run.angle[num_prt] -= 1;
        robot_run.gtas_prt[num_prt] = 0;
      }
      else if((int)robot_run.angle[num_prt] < angle)
      {
        robot_run.angle[num_prt] += 1;
        robot_run.gtas_prt[num_prt] = 0;
      }
      else
      {
        if(0 == robot_run.gtas_prt[num_prt])
        {
          Serial.print("[debug]execute_run: pca: ");
          Serial.print(pca_s);
          Serial.print(". No. ");
          Serial.print(num);
          Serial.print(" get target angel stopped ");
          Serial.println(angle);
          robot_run.gtas_prt[num_prt] = 1;
        }
      }

      robot_run.bs_prt = 0;
      return 0;
    }
    else if(NOTRUN == runornot)
    {
      if(0 == robot_run.bs_prt)
      {
        Serial.println("[debug]execute_run: stopped because of blocking.");
        robot_run.bs_prt = 1;
      }
      return 0;
    }
    else
    {
      Serial.println("[ERROR]execute_run: invalied run status.");
      return -1;
    }
  // }

  return 0;

}

/*
舵机角度输出函数
pca————pca9685芯片选择  1——PCA96851,电路板上方芯片  2————PCA96852,电路板下方芯片
mode————后续开发接口，目前0或1都可以
num————PCA9685输出通道选择 参数为0-11
angle————输出角度（只限于PCA9685在20ms周期条件下输出角度）
vol————当前电压值
*/
int Servo_out(uint16_t pca,uint16_t mode,uint16_t num, int32_t angle, int vol)
{
  int runornot = RUN;

  if(mode==0)
  {
    // runornot = change_run_status(angle, vol);

    execute_run(pca, num, angle, runornot);

    return 0;
  }

  if(mode==1)
  {
    // runornot = change_run_status(angle, vol);

    execute_run(pca, num, angle, runornot);

    return 0;
  }

  return -1;
}
