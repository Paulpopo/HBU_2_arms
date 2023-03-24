#include <Arduino.h>
#include "robot_serial.h"
#include "roubot_two_new_struct.h"

int byte_num = 0;
char received_data[4*ENGINES_NUM+1] = {0};

struct robot_serial
{
    int run_div;                //运行间隔
    int run_cnt;                //间隔计时，达到运行间隔后清0
};
robot_serial robot_serial_ob;

int serial_get_ctl_cmd(char *ctl_data)
{
    int ret = 0;
    int received_end_sign = 0;              //1,已收到结束符；0，收到结束符
    char received_data_catch;

    // 串口接收，当串口无数据且received_data有数据时向外传出数据
    while (Serial.available())
    {
        // 判断received_data的长度，

        received_data_catch = Serial.read();

        // 收到结束标志
        if('\n' == received_data_catch)
        {
            received_end_sign = 1;
            Serial.println("[serial_get_ctl_cmd]recive end sign.");
            break;
        }

        received_data[byte_num] = received_data_catch;
        // Serial.print("byte_num:");
        // Serial.print(byte_num);
        // Serial.print(". received_byte:");
        // Serial.println(received_data[byte_num]);
        byte_num++;
    }

    if(1 == received_end_sign)
    {
        strncpy(ctl_data, received_data, byte_num);
        ret = byte_num;
        // Serial.print("[serial_get_ctl_cmd]ctl_data:");
        // Serial.print(ctl_data);
        // Serial.print(". ctl_data number:");
        // Serial.println(byte_num);
        memset(received_data, 0, sizeof(received_data));
        byte_num = 0;
        return ret;
    }
    else
    {
    }

    return 0;
}

int parse_ctl_data(int *ctl_cmd, int ctl_cmd_num)
{
    int ret = 0;
    int ctl_data_cnt = 0;                               //字符串脚标

    int ctl_data_sign_pos[ctl_cmd_num + 1] = {0};       //符号脚标号
    bool ctl_data_sign_status[ctl_cmd_num] = {0};       //符号极性，"1"为正，"0"为负

    char ctl_data[4*ENGINES_NUM+1] = {0};               //字符串

    int ctl_cmd_cnt = -1;                               //符号脚标号的顺序号

    char data_plus_minus = 0;                           // data_plus_minus=1为正，data_plus_minus=0为负

    int ctl_cmd_abs = 0;
    int sign_step = 1;                                  // 当前计算指向的位距后一个符号的距离

    ret = serial_get_ctl_cmd(ctl_data);
    if(ret > 0)
    {
        Serial.print("[parse_ctl_data]ctl_data:");
        Serial.println(ctl_data);
        // Serial.print(". ctl_data number:");
        // Serial.println(ret);

        // 处理得到的字符串
        // 1、获得符号的位置和极性
        for(ctl_data_cnt = 0; ctl_data_cnt < ret; ctl_data_cnt++)
        {
            if(('+'== ctl_data[ctl_data_cnt]) || ('-'== ctl_data[ctl_data_cnt]))
            {
                // 记录所有符号的脚标，用以数字转换
                ctl_cmd_cnt = ctl_cmd_cnt + 1;
                ctl_data_sign_pos[ctl_cmd_cnt] = ctl_data_cnt;

                // 分别记录正负号，用以数字正负确定
                if('+'== ctl_data[ctl_data_cnt])
                {
                    ctl_data_sign_status[ctl_cmd_cnt] = 1;
                }
                else
                {
                    ctl_data_sign_status[ctl_cmd_cnt] = 0;
                }
                // Serial.print("control cmd sign number: ");
                // Serial.print(ctl_cmd_cnt);
                // Serial.print("  control cmd sign position: ");
                // Serial.println(ctl_data_sign_pos[ctl_cmd_cnt]);
                // Serial.print("  ctl_data_sign_status: ");
                // Serial.println(ctl_data_sign_status[ctl_cmd_cnt]);
            }
        }
        ctl_data_sign_pos[ctl_cmd_cnt + 1] = ret;

        // 2、利用1中的性质处理数字字符
        for(ctl_cmd_cnt = 0; ctl_cmd_cnt < ctl_cmd_num; ctl_cmd_cnt++)
        {
            // 这种计算会导致一组中某些数少1，暂时不知道问题，加了打印中间数值，也是正常的。
            while ((ctl_data[ctl_data_sign_pos[ctl_cmd_cnt+1] - sign_step] >= '0')
                        && (ctl_data[ctl_data_sign_pos[ctl_cmd_cnt+1] - sign_step] <= '9'))
            {
                ctl_cmd_abs = ctl_cmd_abs + (ctl_data[ctl_data_sign_pos[ctl_cmd_cnt+1] - sign_step] - 48) * pow(10, (sign_step - 1));
                sign_step++;
            }
            sign_step = 1;

            if(1 == ctl_data_sign_status[ctl_cmd_cnt])
            {
                ctl_cmd[ctl_cmd_cnt] = ctl_cmd_abs;
            }
            else
            {
                ctl_cmd[ctl_cmd_cnt] = -ctl_cmd_abs;
            }
            ctl_cmd_abs = 0;

            // Serial.print("[parse_ctl_data]ctl_cmd_cnt: ");
            // Serial.print(ctl_cmd_cnt);
            // Serial.print(" ctl_cmd: ");
            // Serial.println(ctl_cmd[ctl_cmd_cnt]);
        }
        ret = ctl_cmd_cnt;
    }

    return ret;
}

void robot_serial_init(void)
{
    robot_serial_ob.run_div = 4;               //运行间隔20ms，解析收到的控制数据，快了串口收发出错
    robot_serial_ob.run_cnt = 0;                //间隔计时
}


// 解析数据执行函数，内部有计数，按指定周期执行
int robot_serial_execute(int *ctl_cmd, int ctl_cmd_num)
{
    int ret = 0;

    if(robot_serial_ob.run_cnt >= robot_serial_ob.run_div)
    {
        robot_serial_ob.run_cnt = 0;
        ret = parse_ctl_data(ctl_cmd, ctl_cmd_num);
        if (ret > 0)
        {
            Serial.print("[debug] robot_serial_execute: ctl_cmd: ");
            for(int i = 0; i < ENGINES_NUM; i++)
            {
                Serial.print(ctl_cmd[i]);
                Serial.print("   ");
            }
            Serial.print('\n');
        }
        if (ret < 0)
        {
            Serial.println("[ERROR] robot_serial_execute: get control cmd number error.");
            return -1;
        }
    }

    robot_serial_ob.run_cnt++;

    return ret;

}

