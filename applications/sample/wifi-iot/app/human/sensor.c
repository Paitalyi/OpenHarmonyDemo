#include <stdio.h>
#include <unistd.h>
#include "ohos_init.h"
#include"cmsis_os2.h"
#include "wifiiot_gpio.h"
#include "wifiiot_gpio_ex.h"
#include"wifiiot_adc.h"

#define HUMAN_SENSOR_CHAN_NAME WIFI_IOT_ADC_CHANNEL_3

unsigned short data = 0;

static void HummanSensorTask(void *arg){

    (void)arg;

    while (1)
    {
    AdcRead(HUMAN_SENSOR_CHAN_NAME,&data, WIFI_IOT_ADC_EQU_MODEL_4,WIFI_IOT_ADC_CUR_BAIS_DEFAULT,0);
    printf("adc:%d\n",data);
    sleep(1);
    }
}

static void HummanSensorDemo(void){
    osThreadAttr_t attr;

    attr.name = "HummanSensorTask";
    attr.attr_bits = 0U;
    attr.cb_mem = NULL;
    attr.cb_size = 0U;
    attr.stack_mem = NULL;
    attr.stack_size = 4096;
    attr.priority = osPriorityNormal;
    if (osThreadNew(HummanSensorTask, NULL,&attr)== NULL){
    printf("[HummanSensorDemo] Falied to create HummanSensorTask!In");
    }
}
APP_FEATURE_INIT(HummanSensorDemo);