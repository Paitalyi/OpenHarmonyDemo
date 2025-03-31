#include <stdio.h>
#include "ohos_init.h"
#include <unistd.h>
#include "wifiiot_gpio.h"
#include "wifiiot_gpio_ex.h"
#include "ohos_types.h"
void leddemo(void)
{
    GpioInit(); // 初始化GPIO设备，使用GPIO管脚前调用

    // 设置GPIO_9功能为IO
    IoSetFunc(WIFI_IOT_IO_NAME_GPIO_9, WIFI_IOT_IO_FUNC_GPIO_9_GPIO);
    // 设置GPI0_9的为输出
    GpioSetDir(WIFI_IOT_IO_NAME_GPIO_9, WIFI_IOT_GPIO_DIR_OUT);
    // 设置GPIO_9输出值为低电平。值为0
    GpioSetOutputVal(WIFI_IOT_IO_NAME_GPIO_9, WIFI_IOT_GPIO_VALUE0);

    printf("led点亮\n"); // 在终端打印LED状态

    usleep(4000000); // 等待4秒

    // 设置GPIO_9输出值为高电平，LED熄灭
    GpioSetOutputVal(WIFI_IOT_IO_NAME_GPIO_9, WIFI_IOT_GPIO_VALUE1);
    printf("led熄灭\n");
}
SYS_RUN(leddemo); // 启动模块，调用leddemo函数