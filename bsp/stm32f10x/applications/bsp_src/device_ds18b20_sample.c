#include <stdlib.h>
#include <rtthread.h>
#include "board.h"
#include "sensor.h"
#include "device_ds18b20.h"
#include "SemInit.h"

#define DS18B20_DATA_PIN   GET_PIN(G, 9)

static void DeviceDs18b20SampleReadTempEntry(void  *parameter) {
  rt_device_t dev = RT_NULL;
  rt_size_t res;

  dev = rt_device_find(parameter);
  if(dev == RT_NULL) {
    rt_kprintf("Can't find device:%s\n", parameter);
    return;
  }
  if(rt_device_open(dev, RT_DEVICE_FLAG_RDWR) != RT_EOK) {
    rt_kprintf("open %s device failed!\n", parameter);
    return;
  }

  rt_device_control(dev, RT_SENSOR_CTRL_SET_ODR, (void *)100);

  while(1) {
    res = rt_device_read(dev, 0, &device_ds18b20.sensor_data_, 1);
    if(res != 1) {
      rt_kprintf("read data failed!size is %d\n", res);
      rt_device_close(dev);
      return;
    } else {
//      if(sensor_data.data.temp >= 0) {
//        rt_kprintf("temp:%3d.%dC, timestamp:%5d\n",
//                   sensor_data.data.temp / 10,
//                   sensor_data.data.temp % 10,
//                   sensor_data.timestamp);
//      } else {
//        rt_kprintf("temp:-%2d.%dC, timestamp:%5d\n",
//                   abs(sensor_data.data.temp / 10),
//                   abs(sensor_data.data.temp % 10),
//                   sensor_data.timestamp);
//      }
      device_ds18b20.data_ = (float)device_ds18b20.sensor_data_.data.temp / 10;
//      printf("temp: %f\r\n", device_ds18b20.data_);
      rt_sem_release(device_ds18b20.ds18b20_sem_);
    }
    rt_thread_mdelay(100);
  }
}

int DeviceDs18b20SampleReadTemp(void) {
  rt_thread_t ds18b20_thread;
  
  device_ds18b20.ds18b20_sem_ = rt_sem_create("ds1820_sem", RT_NULL, RT_IPC_FLAG_FIFO);
  RT_ASSERT(device_ds18b20.ds18b20_sem_);
  
  ds18b20_thread = rt_thread_create("ds18b20_get_temp", DeviceDs18b20SampleReadTempEntry, "temp_ds18b20", 
                                    DS18B20_THREAD_STACK_SIZE, 
                                    DS18B20_THREAD_PRIORITY, 
                                    DS18B20_THREAD_TIMESLICE);
  if(ds18b20_thread != RT_NULL) {
    rt_thread_startup(ds18b20_thread);
  }
  return RT_EOK;
}
//初始化
INIT_APP_EXPORT(DeviceDs18b20SampleReadTemp);

int rt_hw_ds18b20_port(void) {
  struct rt_sensor_config cfg;

  cfg.intf.user_data = (void *)DS18B20_DATA_PIN;
  rt_hw_device_ds18b20_init("ds18b20", &cfg);

  return RT_EOK;
}
//导出命令
//INIT_COMPONENT_EXPORT(rt_hw_ds18b20_port);

INIT_APP_EXPORT(rt_hw_ds18b20_port);

