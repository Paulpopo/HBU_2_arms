#include <Arduino.h>
#include "robot_PCF8591.h"
#include "robot_PCA9685.h"

#define PCA_1   1
#define PCA_2   2

#define CH_BASE 0

int drv_engines(int *cmds, int cmds_num)
{
    int pca = 0;
    int ch = 0;
    int cmd_cnt = 0;
    int vol[5] = {0};
    int cmd_trans_temp[5] = {0};

    if((NULL == cmds) && (0 == cmds_num))
    {
        return -1;
    }

    // !!!! caution !!!!     由于部分电机逆转需要对数组进行转换，故对数据进行拷贝，不对输入指针操作
    for(int i = 0; i < cmds_num; i++)
    {
        cmd_trans_temp[i] = cmds[i];
    }

    // vol[0] = current_GET(10,1,0);
    // vol[1] = current_GET(10,1,1);
    // vol[2] = current_GET(10,1,2);
    // vol[3] = current_GET(10,1,3);
    // vol[4] = current_GET(9,1,3);
    // Serial.print("vol: ");
    // Serial.print(vol[0]);
    // Serial.print("  ");
    // Serial.print(vol[1]);
    // Serial.print("  ");
    // Serial.print(vol[2]);
    // Serial.print("  ");
    // Serial.print(vol[3]);
    // Serial.print("  ");
    // Serial.print(vol[4]);

    for(cmd_cnt = 0; cmd_cnt < cmds_num; cmd_cnt++)
    {
        // 对于右手,肩部舵机1需要初始定位在90度,后续转向0度才能实现上臂抬起,故对右肩部舵机1的输入数据进行处理
        if(0 == cmd_cnt)
        {
            cmd_trans_temp[cmd_cnt] = 90 - cmd_trans_temp[cmd_cnt];
        }

        if(cmd_trans_temp[cmd_cnt] > 90)
        {
            cmd_trans_temp[cmd_cnt] = 90;
        }
        if(cmd_trans_temp[cmd_cnt] < -90)
        {
            cmd_trans_temp[cmd_cnt] = -90;
        }
        // Serial.print("[debug]drv_engines:drive No.");
        // Serial.print(cmd_cnt);
        // Serial.print(" engine. angle: ");
        // Serial.println(cmd_trans_temp[cmd_cnt]);
        //驱动：1、一直转动，直到顶到物体；2、一直转动，直到达到指定位置
        // Servo_out(PCA_2, 1, (CH_BASE + cmd_cnt), cmd_trans_temp[cmd_cnt]);
        Servo_out(PCA_2, 1, (CH_BASE + cmd_cnt), cmd_trans_temp[cmd_cnt], vol[cmd_cnt]);
    }

    return 0;
}