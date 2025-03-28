#include <stdio.h>
#include <unistd.h>

#include "ohos_init.h"
#include "cmsis_os2.h"
#include "wifiiot_gpio.h"
#include "wifiiot_gpio_ex.h"
#include "wifiiot_adc.h"
#include "wifiiot_errno.h"

#define GAS_SENSOR_CHAN_NAME WIFI_IOT_ADC_CHANNEL_5

unsigned short data = 0; //保存读取到的燃气值

static void Mq2DemoTask(void *arg)
{
    (void)arg;
    GpioInit();
    while (1)
    {
        //调用AdcRead读取值
        if (AdcRead(GAS_SENSOR_CHAN_NAME, &data, WIFI_IOT_ADC_EQU_MODEL_4, WIFI_IOT_ADC_CUR_BAIS_DEFAULT, 0) == WIFI_IOT_SUCCESS)
        {
            printf("gas:%d ppm\n", data);
        }

        sleep(1);
    }
}

static void Mq2Demo(void)
{
    osThreadAttr_t attr;
    attr.name = "Mq2DemoTask";
    attr.attr_bits = 0U;
    attr.cb_mem = NULL;
    attr.cb_size = 0U;
    attr.stack_mem = NULL;
    attr.stack_size = 4096;
    attr.priority = osPriorityNormal;
    if (osThreadNew(Mq2DemoTask, NULL, &attr) == NULL)
    {
        printf("[EnvironmentDemo] Falied to create Mq2DemoTask!\n");
    }
}

APP_FEATURE_INIT(Mq2Demo);