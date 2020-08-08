#ifndef __DEVICE_DS18B20_H__
#define __DEVICE_DS18B20_H__

#include <rthw.h>
#include <rtthread.h>
#include <rtdevice.h>
#include "sensor.h"

#define CONNECT_SUCCESS  0
#define CONNECT_FAILED   1

typedef struct {
  rt_base_t pin_;
  rt_mutex_t lock_;
}Device_Ds18b20;

int rt_hw_device_ds18b20_init(const char *name, struct rt_sensor_config *cfg);



#endif

