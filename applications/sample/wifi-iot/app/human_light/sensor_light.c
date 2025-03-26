#include <stdio.h>
#include <unistd.h>

#include"ohos_init.h"
#include"cmsis_os2.h"
#include"wifiiot_gpio.h"
#include "wifiiot_gpio_ex.h"

void init(void)
{
    GpioInit();
    //设置红色，蓝色，绿色LEDIO为输出状态
    IoSetFunc(WIFI_IOT_IO_NAME_GPIO_10, WIFI_IOT_IO_FUNC_GPIO_10_GPIO);
    GpioSetDir(WIFI_IOT_IO_NAME_GPIO_10,WIFI_IOT_GPIO_DIR_OUT);

    IoSetFunc(WIFI_IOT_IO_NAME_GPIO_11,WIFI_IOT_IO_FUNC_GPIO_11_GPIO);
    GpioSetDir(WIFI_IOT_IO_NAME_GPIO_11,WIFI_IOT_GPIO_DIR_OUT);

    IoSetFunc(WIFI_IOT_IO_NAME_GPIO_12,WIFI_IOT_IO_FUNC_GPIO_12_GPIO);
    GpioSetDir(WIFI_IOT_IO_NAME_GPIO_12,WIFI_IOT_GPIO_DIR_OUT);

    IoSetFunc(WIFI_IOT_IO_NAME_GPIO_9,WIFI_IOT_IO_FUNC_GPIO_9_GPIO);
    GpioSetDir(WIFI_IOT_IO_NAME_GPIO_9,WIFI_IOT_GPIO_DIR_IN);

    IoSetFunc(WIFI_IOT_IO_NAME_GPIO_7,WIFI_IOT_IO_FUNC_GPIO_7_GPIO);
    GpioSetDir(WIFI_IOT_IO_NAME_GPIO_7,WIFI_IOT_GPIO_DIR_IN);
    IoSetPull(WIFI_IOT_IO_NAME_GPIO_7, WIFI_IOT_IO_PULL_UP);
}

static void HummanSensorLightTask(void *arg)
{
    (void)arg;

    init();
    WifiIotGpioValue rel = 0;
    while (1)
    {
    GpioGetInputVal(WIFI_IOT_IO_NAME_GPIO_7, &rel);
    GpioSetOutputVal(WIFI_IOT_IO_NAME_GPIO_10, (int)rel);
    GpioSetOutputVal(WIFI_IOT_IO_NAME_GPIO_11, (int)rel);
    GpioSetOutputVal(WIFI_IOT_IO_NAME_GPIO_12, (int)rel);
    usleep(20 * 1000);
    }
}
static void HummanSensorLightDemo(void)
{
    osThreadAttr_t attr;
    attr.name = "HummanSensorLightTask";
    attr.attr_bits = 0U;
    attr.cb_mem = NULL;
    attr.cb_size = 0U;
    attr.stack_mem = NULL;
    attr.stack_size = 4096;
    attr.priority = osPriorityNormal;
    if (osThreadNew(HummanSensorLightTask, NULL, &attr) == NULL){
    printf("[HummanSensorDemo] Falied to create HummanSensorTask!In");
    }
}
APP_FEATURE_INIT(HummanSensorLightDemo);