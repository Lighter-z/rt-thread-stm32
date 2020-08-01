#ifndef __BSP_ADC_H
#define __BSP_ADC_H

#include "applications/main.h"

#define     ADC_DEV_NAME        "adc1"              /* 定义adc设备的名字 */
#define     ADC_DEV_CHANNEL      13                 /* 定义adc设备的通道 */
#define     REFER_VOLTAGE        330                /* 定义adc设备的参考电压，比真实的硬件电压扩大了十倍，方便计算处理 */
#define     CONVERT_BITS         (1 << 12)          /* 定义adc设备的转换精度 */

#define BSP_ADC_PRIORITY    9						/* 定义bsp_key线程的优先级 */

/* 声明函数 */
static void bsp_adc_thread_entry(void *parameter);
static void bsp_adc_sample(int arc, char *argv[]);

#endif
