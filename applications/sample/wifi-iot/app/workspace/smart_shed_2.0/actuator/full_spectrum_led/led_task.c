#include <stdio.h>
#include <unistd.h>

#include "ohos_init.h"
#include "cmsis_os2.h"
#include "wifiiot_gpio.h"
#include "wifiiot_gpio_ex.h"
#include "wifiiot_pwm.h"
#include "mqtt_utils.h" // 使用全局变量led_level

#define RED_LED_PIN_NAME WIFI_IOT_IO_NAME_GPIO_10
#define RED_LED_PIN_FUNC WIFI_IOT_IO_FUNC_GPIO_10_GPIO
#define GREEN_LED_PIN_NAME WIFI_IOT_IO_NAME_GPIO_11
#define GREEN_LED_PIN_FUNC WIFI_IOT_IO_FUNC_GPIO_11_GPIO
#define BLUE_LED_PIN_NAME WIFI_IOT_IO_NAME_GPIO_12
#define BLUE_LED_PIN_FUNC WIFI_IOT_IO_FUNC_GPIO_12_GPIO

#define PWM_FREQ_DIVITION 64000

void led_task_init(void){
    GpioInit(); //初始化GPIO管脚
    IoSetFunc(RED_LED_PIN_NAME, WIFI_IOT_IO_FUNC_GPIO_10_PWM1_OUT);
    IoSetFunc(GREEN_LED_PIN_NAME, WIFI_IOT_IO_FUNC_GPIO_11_PWM2_OUT);
    IoSetFunc(BLUE_LED_PIN_NAME, WIFI_IOT_IO_FUNC_GPIO_12_PWM3_OUT);

    PwmInit(WIFI_IOT_PWM_PORT_PWM1); //R
    PwmInit(WIFI_IOT_PWM_PORT_PWM2); //G
    PwmInit(WIFI_IOT_PWM_PORT_PWM3); //B
}

void led_thread(void* arg){
    (void)arg;
    led_task_init();

    while(1){
        if(led_level >= 1 && led_level <= 100){
            PwmStart(WIFI_IOT_PWM_PORT_PWM1, PWM_FREQ_DIVITION / 150 * led_level, PWM_FREQ_DIVITION); // Port DutyCycle Freq
            PwmStart(WIFI_IOT_PWM_PORT_PWM2, PWM_FREQ_DIVITION / 150 * led_level, PWM_FREQ_DIVITION);
            PwmStart(WIFI_IOT_PWM_PORT_PWM3, PWM_FREQ_DIVITION / 150 * led_level, PWM_FREQ_DIVITION);
        }else{
            PwmStop(WIFI_IOT_PWM_PORT_PWM1);
            PwmStop(WIFI_IOT_PWM_PORT_PWM2);
            PwmStop(WIFI_IOT_PWM_PORT_PWM3);
        }
        sleep(1); //休眠 防止操作过于频繁
    }
}

void led_task(void){
    osThreadAttr_t attr;
    attr.name = "led_thread";
    attr.attr_bits = 0U;
    attr.cb_mem = NULL;
    attr.cb_size = 0U;
    attr.stack_mem = NULL;
    attr.stack_size = 4096;
    attr.priority = osPriorityNormal;
    if (osThreadNew((osThreadFunc_t)led_thread, NULL, &attr) == NULL) {
        printf("[led_task] Failed to create led_thread!\n");
    }
}