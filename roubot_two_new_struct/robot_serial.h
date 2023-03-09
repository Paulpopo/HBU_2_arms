#ifndef __ROBOT_SERIAL_H__
#define __ROBOT_SERIAL_H__

int serial_get_ctl_cmd(char *ctl_data);
int parse_ctl_data(int *ctl_cmd, int ctl_cmd_num);

void robot_serial_init(void);
int robot_serial_execute(int *ctl_cmd, int ctl_cmd_num);


#endif