#ifndef __DEVICE_DS18B20_H__
#define __DEVICE_DS18B20_H__

#include <rthw.h>
#include <rtthread.h>
#include <rtdevice.h>
#include "sensor.h"

#define CONNECT_SUCCESS  0
#define CONNECT_FAILED   1

#define DS18B20_THREAD_STACK_SIZE       (1024  * 2)                          //线程栈大小
#define DS18B20_THREAD_PRIORITY         (RT_THREAD_PRIORITY_MAX / 2 )     //优先级
//#define ONENET_THREAD_TIMESLICE        5                                    //时间片
#define DS18B20_THREAD_TIMESLICE        20                                    //时间片

typedef struct {
  rt_base_t pin_;
  rt_mutex_t lock_;
  rt_sem_t ds18b20_sem_;
  
  struct rt_sensor_data sensor_data_;
  float data_;
}Device_Ds18b20;

extern Device_Ds18b20 device_ds18b20;

int rt_hw_device_ds18b20_init(const char *name, struct rt_sensor_config *cfg);

int rt_hw_ds18b20_port(void);
int DeviceDs18b20SampleReadTemp(void);

#endif

