#include <stdio.h>
#include <unistd.h>
#include "ohos_init.h"
#include "cmsis_os2.h"
#include "wifiiot_gpio.h"
#include "wifiiot_gpio_ex.h"
#include "hi_gpio.h"

static void OnButtonPressed(char *arg)
{
    (void)arg; // 用于‌显式地忽略未使用的参数‌，以避免编译器警告
    printf("Key S3 pressed\r\n");
}
static void StartS3Task(void)
{
    GpioInit();

    IoSetFunc(WIFI_IOT_IO_NAME_GPIO_8, WIFI_IOT_IO_FUNC_GPIO_8_GPIO); // 设置GPIO8的功能为IO
    GpioSetDir(WIFI_IOT_IO_NAME_GPIO_8, WIFI_IOT_GPIO_DIR_IN); // 设置GPIO8的输入输入方向（dir）为输入，即输入模式
    IoSetPull(WIFI_IOT_IO_NAME_GPIO_8, WIFI_IOT_IO_PULL_UP); // GPIO 引脚的上拉/下拉电阻可以确保引脚在未连接外部设备时保持一个确定的电平状态（高电平或低电平）
                                                             // 这从而避免误触发或不稳定的信号

    GpioRegisterIsrFunc(WIFI_IOT_IO_NAME_GPIO_8, WIFI_IOT_INT_TYPE_EDGE, WIFI_IOT_GPIO_EDGE_FALL_LEVEL_LOW,
                        OnButtonPressed, NULL); // WIFI_IOT_INT_TYPE_EDGE表示在edge时中断
                                                // WIFI_IOT_GPIO_EDGE_FALL_LEVEL_LOW表示在edge_fall或level_low的时候中断（取决于前一个参数）
    while (1)
    {
        sleep(1);
    }
}
static void Starts3Demo(void)
{
    osThreadAttr_t attr;

    attr.name = "s3Task";
    attr.attr_bits = 0U;
    attr.cb_mem = NULL;
    attr.cb_size = 0U;
    attr.stack_mem = NULL;
    attr.stack_size = 1024;
    attr.priority = osPriorityNormal;

    if (osThreadNew((osThreadFunc_t)StartS3Task, NULL, &attr) == NULL)
    {
        printf("[Starts3Task] Falied to create StartS3Task!\n");
    }
    printf("IrIn[StartS3Task] Succ to create Starts3Task!In");
}
APP_FEATURE_INIT(Starts3Demo);