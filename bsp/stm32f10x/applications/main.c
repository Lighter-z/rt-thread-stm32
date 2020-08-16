/*
 * Copyright (c) 2006-2018, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2018-11-06     SummerGift   first version
 *
 * esp8266-01
 * 
 *
 * 3.3V  -------  3.3V
 * EN    -------  3.3V
 * TX    -------  PA3
 * RX    -------  PA2
 * GND   -------  GND
 *
 *
 */

#include "main.h"


/* defined the LED0 pin: PB5 */
#define LED0_PIN    GET_PIN(B, 5)

rt_sem_t mqqt_init_sem;

static rt_thread_t onenet_mqtt_init_thread;         /* mqtt初始化的线程的句柄 */

static void onenet_mqtt_init_entry(void *parameter)
{
    uint8_t onenet_mqtt_init_failed_times;
    
    /* mqtt初始化 */
    while (1)
    {
        if (!onenet_mqtt_init())
        {
            /* mqtt初始化成功之后，释放信号量告知onenet_upload_data_thread线程可以上传数据了 */
            rt_sem_release(mqqt_init_sem);
            return;
        }
        rt_thread_mdelay(100);
        LOG_E("onenet mqtt init failed %d times", onenet_mqtt_init_failed_times++);
    }
}

static void OneNetUploadData(void) {
  
  mqqt_init_sem = rt_sem_create("mqtt_sem", RT_NULL, RT_IPC_FLAG_FIFO);
  RT_ASSERT(mqqt_init_sem);
  
  onenet_mqtt_init_thread = rt_thread_create("mqttinit", onenet_mqtt_init_entry, RT_NULL,
                                               1024, RT_THREAD_PRIORITY_MAX / 2 - 2, 20);
  if (onenet_mqtt_init_thread != RT_NULL)
  {
      rt_thread_startup(onenet_mqtt_init_thread);
  }
}

int main(void)
{
  rt_err_t ret = RT_NULL;
  
//  ret = rt_hw_ds18b20_port();
//  if(ret != RT_EOK) {
//    rt_kprintf("ds18b20 init failed \r\n");
//  }
//  DeviceDs18b20SampleReadTemp();
  
//  onenet_mqtt_init();
//  rt_kprintf("onenet mqtt init = %d\r\n", ret);
  OneNetUploadData();
  OneNetUploadCycle();
  
  return RT_EOK;
}
