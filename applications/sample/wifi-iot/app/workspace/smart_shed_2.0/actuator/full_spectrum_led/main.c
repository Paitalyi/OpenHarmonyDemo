#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "ohos_init.h"
#include "cmsis_os2.h"
#include "led_task.h"
#include "sub_task.h"

static void ss_thread(void* arg){
    (void)arg;

    sleep(1); //防止系统未初始化完成就执行
    printf("Smart Shed 2.0 is running!");

    led_task();
    // oled_task();
    sub_task();
}

void ss_entry(void){
    osThreadAttr_t attr;
    attr.name = "ss_thread";
    attr.attr_bits = 0U;
    attr.cb_mem = NULL;
    attr.cb_size = 0U;
    attr.stack_mem = NULL;
    attr.stack_size = 1024 * 4;
    attr.priority = osPriorityNormal;
    
    if(osThreadNew((osThreadFunc_t)ss_thread, NULL, &attr) == NULL){
        printf("[Smart Shed 2.0] Failed to create ss_thread\n");
    }
    SYS_RUN(ss_entry);
}