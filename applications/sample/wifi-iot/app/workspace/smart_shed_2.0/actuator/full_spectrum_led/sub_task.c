#include "cmsis_os2.h"
#include <unistd.h>
#include <stdio.h>
#include "wifi_utils.h"
#include "mqtt_utils.h"

static void sub_thread(void *arg)
{
    (void)arg;
    connect_wifi(); //连接wifi
    sleep(1);       //等待wifi
    mqtt_connect(); //调用mqtt
}

void sub_task(void)
{
    osThreadAttr_t attr;

    attr.name = "sub_task";
    attr.attr_bits = 0U;
    attr.cb_mem = NULL;
    attr.cb_size = 0U;
    attr.stack_mem = NULL;
    attr.stack_size = 4096;
    attr.priority = osPriorityNormal;

    if (osThreadNew((osThreadFunc_t)sub_thread, NULL, &attr) == NULL)
    {
        printf("[sub_task] Falied to create sub_thread!\n");
    }
}
