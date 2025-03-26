#include <stdio.h>
#include "ohos_init.h"
#include <unistd.h>
#include "wifiiot_gpio.h"
#include "wifiiot_gpio_ex.h"
#include "ohos_types.h"
void leddemo(void)
{
    GpioInit(); // ��ʼ��GPIO�豸��ʹ��GPIO�ܽ�ǰ����

    // ����GPIO_9����ΪIO
    IoSetFunc(WIFI_IOT_IO_NAME_GPIO_9, WIFI_IOT_IO_FUNC_GPIO_9_GPIO);
    // ����GPI0_9��Ϊ���
    GpioSetDir(WIFI_IOT_IO_NAME_GPIO_9, WIFI_IOT_GPIO_DIR_OUT);
    // ����GPIO_9���ֵΪ�͵�ƽ��ֵΪ0
    GpioSetOutputVal(WIFI_IOT_IO_NAME_GPIO_9, WIFI_IOT_GPIO_VALUE0);

    printf("led����\n"); // ���ն˴�ӡLED״̬

    usleep(4000000); // �ȴ�4��

    // ����GPIO_9���ֵΪ�ߵ�ƽ��LEDϨ��
    GpioSetOutputVal(WIFI_IOT_IO_NAME_GPIO_9, WIFI_IOT_GPIO_VALUE1);
    printf("ledϨ��\n");
}
SYS_RUN(leddemo); // ����ģ�飬����leddemo����