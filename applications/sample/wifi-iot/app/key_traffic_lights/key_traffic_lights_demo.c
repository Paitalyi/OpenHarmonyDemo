#include <stdio.h>
#include <unistd.h>
#include "ohos_init.h"
#include "cmsis_os2.h"
#include "wifiiot_gpio.h"
#include "wifiiot_gpio_ex.h"
#include "wifiiot_watchdog.h"
#include "hi_gpio.h"

static int g_currentBright = -1;
WifiIotGpioIdx pins[] = {WIFI_IOT_GPIO_IDX_10, WIFI_IOT_GPIO_IDX_11, WIFI_IOT_GPIO_IDX_12};

void TrafficLightInit(void)
{
    GpioInit();
    IoSetFunc(WIFI_IOT_IO_NAME_GPIO_10, WIFI_IOT_IO_FUNC_GPIO_10_GPIO); // red
    GpioSetDir(WIFI_IOT_IO_NAME_GPIO_10, WIFI_IOT_GPIO_DIR_OUT);

    IoSetFunc(WIFI_IOT_IO_NAME_GPIO_11, WIFI_IOT_IO_FUNC_GPIO_11_GPIO); // green
    GpioSetDir(WIFI_IOT_IO_NAME_GPIO_11, WIFI_IOT_GPIO_DIR_OUT);

    IoSetFunc(WIFI_IOT_IO_NAME_GPIO_12, WIFI_IOT_IO_FUNC_GPIO_12_GPIO); // yellow
    GpioSetDir(WIFI_IOT_IO_NAME_GPIO_12, WIFI_IOT_GPIO_DIR_OUT);

    IoSetFunc(WIFI_IOT_IO_NAME_GPIO_8, WIFI_IOT_IO_FUNC_GPIO_8_GPIO); // button
    GpioSetDir(WIFI_IOT_IO_NAME_GPIO_8, WIFI_IOT_GPIO_DIR_IN);
    IoSetPull(WIFI_IOT_IO_NAME_GPIO_8, WIFI_IOT_IO_PULL_UP);
}
static void OnButtonPressed(char *arg)
{
    (void)arg;
    GpioUnregisterIsrFunc(WIFI_IOT_IO_NAME_GPIO_8); //禁止中断或注销中断
    g_currentBright++;
    if (g_currentBright == 3)
        g_currentBright = 0;
    for (int i = 0; i < 3; i++)
    {
        i == g_currentBright ? GpioSetOutputVal(pins[i], WIFI_IOT_GPIO_VALUE1) : GpioSetOutputVal(pins[i], WIFI_IOT_GPIO_VALUE0);
    }
    usleep(500 * 1000); // 延时500ms
    GpioRegisterIsrFunc(WIFI_IOT_IO_NAME_GPIO_8, WIFI_IOT_INT_TYPE_EDGE, WIFI_IOT_GPIO_EDGE_FALL_LEVEL_LOW,
                        OnButtonPressed, NULL);
}

static void *TrafficLightTask(const char *arg)
{
    (void)arg;

    TrafficLightInit();

    for (int i = 0; i < 4; i++) // 启动时的快闪
    {
        for (unsigned int j = 0; j < 3; j++)
        {
            GpioSetOutputVal(pins[j], WIFI_IOT_GPIO_VALUE1);
            usleep(200 * 1000);

            GpioSetOutputVal(pins[j], WIFI_IOT_GPIO_VALUE0);
            usleep(100 * 1000);
        }
    }
    WatchDogDisable();
    GpioRegisterIsrFunc(WIFI_IOT_IO_NAME_GPIO_8, WIFI_IOT_INT_TYPE_EDGE, WIFI_IOT_GPIO_EDGE_FALL_LEVEL_LOW,
                        OnButtonPressed, NULL);

    while (1)
    {
        usleep(20 * 1000);
    }
    return NULL;
}

static void StartTrafficLightTask(void)
{
    osThreadAttr_t attr;

    attr.name = "TrafficLightTask";
    attr.attr_bits = 0U;
    attr.cb_mem = NULL;
    attr.cb_size = 0U;
    attr.stack_mem = NULL;
    attr.stack_size = 1024;
    attr.priority = osPriorityNormal;

    if (osThreadNew((osThreadFunc_t)TrafficLightTask, NULL, &attr) == NULL)
    {
        printf("[LedExample] Falied to create TrafficLightTask!\n");
    }
    printf("\r\n[LedExample] Succ to create TrafficLightTask!\n");
}

APP_FEATURE_INIT(StartTrafficLightTask);
