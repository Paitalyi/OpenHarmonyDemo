#include <stdio.h>
#include <unistd.h>
#include "ohos_init.h"
#include "cmsis_os2.h"
#include "wifiiot_gpio.h"
#include "wifiiot_gpio_ex.h"
#include "wifiiot_watchdog.h"
#include "wifiiot_pwm.h"
#include "hi_pwm.h"

static const uint16_t g_tuneFreqs[] = {
    0,     // 40M Hz 对应的分频系数：
    38223, // 1046.5 (C6)
    34052, // 1174.7 (D6)
    30338, // 1318.5 (E6)
    28635, // 1396.9 (F6)
    25511, // 1568 (G6)
    22728, // 1760 (A6)
    20249, // 1975.5 (B6)
    19111, // 2093 (C7)
    17026, // 2349.3 (D7)
    15169, // 2637 (E7)
    14317, // 2793.8 (F7)
    12755, // 3136 (G7)
    11364, // 3520 (A7)
    10124, // 3951 (B7)
    19111  // 2093 (C7) 高八度的1
};

// 《世末歌者》简谱
static const uint8_t g_scoreNotes[] = {
    5, 5, 6, 5, 1, 2, 3, 3, 2, 1, 5, 5, 6, 5, 2, 3, 
    4, 4, 3, 2, 1, 1, 2, 3, 4, 3, 2, 1, 2, 5, 5, 5, 
    6, 5, 1, 2, 3, 3, 2, 1, 5, 5, 6, 5, 2, 3, 4, 4, 
    3, 2, 1, 1, 2, 3, 4, 3, 2, 1, 1, 8, 8, 8, 8, 8
};

// 曲谱时值
static const uint8_t g_scoreDurations[] = {
    4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4,
    4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 8, 4, 4, 4,
    4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4,
    4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 8, 4, 4, 4, 4, 8
};

static void *BeeperMusicTask(const char *arg)
{
    (void)arg;
    GpioInit();
    // 蜂鸣器引脚 设置为 PWM功能
    IoSetFunc(WIFI_IOT_IO_NAME_GPIO_9, WIFI_IOT_IO_FUNC_GPIO_9_PWM0_OUT);
    PwmInit(WIFI_IOT_PWM_PORT_PWM0);
    WatchDogDisable();

    printf("BeeperMusicTask start!\r\n");
    hi_pwm_set_clock(PWM_CLK_XTAL); // 设置时钟源为晶体时钟（40MHz，默认时钟源160MHz）
    while (1)
    {
        for (size_t i = 0; i < sizeof(g_scoreNotes) / sizeof(g_scoreNotes[0]); i++)
        {
            uint32_t tune = g_scoreNotes[i];                            // 音符
            uint16_t freqDivisor = g_tuneFreqs[tune];                   //获取 频率
            uint32_t tuneInterval = g_scoreDurations[i] * (125 * 1000); // 音符时间   ，节拍，
            printf("%d %d %d %d\r\n", tune, (40 * 1000 * 1000) / freqDivisor, freqDivisor, tuneInterval);
            PwmStart(WIFI_IOT_PWM_PORT_PWM0, freqDivisor / 2, freqDivisor);
            usleep(tuneInterval); //持续响tuneInterval时间，
            PwmStop(WIFI_IOT_PWM_PORT_PWM0);
        }
        usleep(2000 * 1000);
    }
    return NULL;
}

static void StartBeepMusicTask(void)
{
    osThreadAttr_t attr;

    //设置新线程属性
    attr.name = "BeeperMusicTask";
    attr.attr_bits = 0U;
    attr.cb_mem = NULL;
    attr.cb_size = 0U;
    attr.stack_mem = NULL;
    attr.stack_size = 1024;
    attr.priority = osPriorityNormal;

    //创建线程，在新的线程中运行BeeperMusicTask函数
    if (osThreadNew((osThreadFunc_t)BeeperMusicTask, NULL, &attr) == NULL)
    {
        printf("[BeeperMusicTask] Falied to create BeeperMusicTask!\n");
    }
}

APP_FEATURE_INIT(StartBeepMusicTask);