#ifndef __BSP_HWTIMER_H
#define __BSP_HWTIMER_H

#include "applications/main.h"

#define	    BEEP_PIN        GET_PIN(B, 8)       /* 定义蜂鸣器引脚 */
#define     HWTIMER_DEV_NAME     "timer4"       /* 定义硬件定时器设备名字 */

/* 定义bsp_sem线程的优先级 */
#define BSP_SEM_PRIORITY    10

#define BSP_SEM_STACK_SIZE  512

#define BSP_SEM_TICK        10

/* 声明相关函数 */
//static rt_err_t bsp_hwtimer_cb(rt_device_t dev, rt_size_t size);
//static rt_uint8_t bsp_hwtimer_sample(int argc, char *argv[]);
//static void bsp_sem_thread_entry(void *parameter);
#endif

