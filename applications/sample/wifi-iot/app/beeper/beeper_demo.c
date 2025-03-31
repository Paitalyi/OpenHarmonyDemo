#include <stdio.h>
#include <unistd.h>
#include "ohos_init.h"
#include "cmsis_os2.h"
#include "wifiiot_gpio.h"
#include "wifiiot_gpio_ex.h"
#include "wifiiot_watchdog.h"
#include "wifiiot_pwm.h"
#include "hi_pwm.h"

void init(void)
{
    Gpioinit();
    IoSetFunc(WIFI_IOT_IO_NAME_GPIO_9, WIFI_IOT_IO_FUNC_GPIO_9_PWM0_OUT); // 蜂鸣器引脚设置为PWM输出
    PwmInit(WIFI_IOT_PWM_PORT_PWM0);                                      // 初始化PWM0
}

void BeeperDemo(void)
{
    init();
    while (1)
    {
        uint16_t freqDivisor = 34052;// 占空比
        PwmStart(WIFI_IOT_PWM_PORT_PWM0, freqDivisor / 2, freqDivisor);
        usleep(2000 * 1000);
        PwmStop(WIFI_IOT_PWM_PORT_PWM0);
        usleep(1000 * 1000);
    }
}

SYS_RUN(BeeperDemo);