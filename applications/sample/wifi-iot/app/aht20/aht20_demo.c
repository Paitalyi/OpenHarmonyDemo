#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>

#include "ohos_init.h"
#include "cmsis_os2.h" // OS适配层
#include "wifiiot_i2c.h"
#include "wifiiot_gpio.h"
#include "wifiiot_gpio_ex.h"
#include "wifiiot_errno.h" // 定义Wifi module使用的Error code
#include "aht20.h"

#define AHT20_BAUDRATE 400 * 1000
#define AHT20_I2C_IDX WIFI_IOT_I2C_IDX_0

void init(void){
    GpioInit();
    IoSetFunc(WIFI_IOT_IO_NAME_GPIO_13, WIFI_IOT_IO_FUNC_GPIO_13_I2C0_SDA);
    IoSetFunc(WIFI_IOT_IO_NAME_GPIO_14, WIFI_IOT_IO_FUNC_GPIO_14_I2C0_SCL);
    I2cInit(AHT20_I2C_IDX, AHT20_BAUDRATE);
}

static void Aht20Task(void* arg){
    (void)arg;
    uint32_t retval; //retval系统调用的返回值
    float humidity = 0.0f; //湿度
    float temperature = 0.0f; //温度

    while(AHT20_Calibrate() != WIFI_IOT_SUCCESS){// 校准AHT20
        printf("AHT20 sensor init failed!\r\n");
        usleep(1000); //1000us = 1ms
    }

    while(1){
        retval = AHT20_StartMeasure();
        if(retval != WIFI_IOT_SUCCESS){
            printf("Trigger AHT20 measure failed!\r\n");
        }else{
            retval = AHT20_GetMeasureResult(&temperature, &humidity);
            printf("Temp: %.2f,Humi: %.2f\r\n", temperature, humidity);
        }
        sleep(1); //1s 防止频繁测量
    }
}

static void Aht20Demo(void){
    osThreadAttr_t attr;
    init();
    attr.name = "Aht20Demo";
    attr.attr_bits = 0U;
    attr.cb_mem = NULL;
    attr.cb_size = 0U;
    attr.stack_mem = NULL;
    attr.stack_size = 1024 * 4;
    attr.priority = osPriorityNormal;

    if (osThreadNew((osThreadFunc_t)Aht20Task, NULL, &attr) == NULL) {
        printf("[Aht20Demo] Failed to create Aht20Demo!\n");
    }
}

APP_FEATURE_INIT(Aht20Demo);