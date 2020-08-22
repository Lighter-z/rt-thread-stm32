#include "device_ds18b20.h"
#include "sensor.h"
#include "board.h"
#include <rtdbg.h>

#define DBG_TAG "sensor.device.ds18b20"
#define DBG_LVL DBG_INFO

#define SENSOR_TEMP_RANGE_MAX (125)
#define SENSOR_TEMP_RANGE_MIN (-55)

Device_Ds18b20 device_ds18b20;

RT_WEAK void rt_hw_us_delay(rt_uint32_t us) {
  rt_uint32_t delta;

  us = us * (SysTick->LOAD / (1000000 / RT_TICK_PER_SECOND));
  delta = SysTick->VAL;

  while(delta - SysTick->VAL < us) continue;
}

static void DeviceDs18b20Reset(rt_base_t pin) {
  rt_pin_mode(pin, PIN_MODE_OUTPUT);
  rt_pin_write(pin, PIN_LOW);
  rt_hw_us_delay(780);
  rt_pin_write(pin, PIN_HIGH);
  rt_hw_us_delay(40);
}

static uint8_t DeviceDs18b20Connect(rt_base_t pin) {
  uint8_t retry = 0;

  rt_pin_mode(pin, PIN_MODE_INPUT);

  while(rt_pin_read(pin) && retry < 200) {
    retry++;
    rt_hw_us_delay(1);
  }

  if(retry >= 200) {
    return CONNECT_FAILED;
  } else {
    retry = 0;
  }

  while(!rt_pin_read(pin) && retry < 240) {
    retry++;
    rt_hw_us_delay(1);
  }

  if(retry >= 240) {
    return CONNECT_FAILED;
  } 

  return CONNECT_SUCCESS;
}

static uint8_t DeviceDs18b20ReadBit(rt_base_t pin) {
  uint8_t data;

  rt_pin_mode(pin, PIN_MODE_OUTPUT);
  rt_pin_write(pin, PIN_LOW);
  rt_hw_us_delay(2);
  rt_pin_write(pin, PIN_HIGH);
  rt_pin_mode(pin, PIN_MODE_INPUT);

  /* maybe 12us, maybe 5us, whatever...I have no idea */
  rt_hw_us_delay(5);

  if(rt_pin_read(pin)) {
    data = 1;
  } else {
    data = 0;
  }

  rt_hw_us_delay(50);

  return data;
}

static uint8_t DeviceDs18b20ReadByte(rt_base_t pin) {
  uint8_t i, j, data;
  data = 0;

  for(i = 1; i <= 8; i++) {
    j = DeviceDs18b20ReadBit(pin);
    data = (j << 7) | (data >> 1);
  }
  return data;
}

static void DeviceDs18b20WriteByte(rt_base_t pin, uint8_t data) {
  uint8_t i;
  uint8_t res;

  rt_pin_mode(pin, PIN_MODE_OUTPUT);

  for(i = 1; i <= 8; i++) {
    res = data & 0x01;
    data = data >> 1;

    if(res) {
      rt_pin_write(pin, PIN_LOW);
      rt_hw_us_delay(2);
      rt_pin_write(pin, PIN_HIGH);
      rt_hw_us_delay(60);
    } else {
      rt_pin_write(pin, PIN_LOW);
      rt_hw_us_delay(60);
      rt_pin_write(pin, PIN_HIGH);
      rt_hw_us_delay(2);
    }
  }
}

static void DeviceDs18b20Start(rt_base_t pin) {
  DeviceDs18b20Reset(pin);
  DeviceDs18b20Connect(pin);
  DeviceDs18b20WriteByte(pin, 0xcc);  /* skip rom */
  DeviceDs18b20WriteByte(pin, 0x44);  /* convert */
}

uint8_t DeviceDs18b20Init(rt_base_t pin) {
  uint8_t ret = 0;
  
  DeviceDs18b20Reset(pin);
  ret = DeviceDs18b20Connect(pin);

  return ret;
}

int32_t DeviceDs18b20GetTemp(rt_base_t pin) {
  uint8_t data_th;
  uint8_t data_tl;
  int32_t temp;

  DeviceDs18b20Start(pin);
  DeviceDs18b20Init(pin);
  DeviceDs18b20WriteByte(pin, 0xcc);
  DeviceDs18b20WriteByte(pin, 0xbe);
  data_tl = DeviceDs18b20ReadByte(pin);
  data_th = DeviceDs18b20ReadByte(pin);
  if(data_th > 7) {
    data_th =~ data_th;
    data_tl =~ data_tl;
    temp = data_th;
    temp <<= 8;
    temp += data_tl;
    temp = (int32_t)(temp * 0.0625 * 10 + 0.5);
    return -temp;
  } else {
    temp = data_th;
    temp <<= 8;
    temp += data_tl;
    temp = (int32_t)(temp * 0.0625 * 10 + 0.5);
    return temp;
  }
}

static rt_size_t _DeviceDs18b20PollingGetData(rt_sensor_t sensor, struct rt_sensor_data *data) {
  rt_uint32_t temp_x10;
  if(sensor->info.type == RT_SENSOR_CLASS_TEMP) {
    temp_x10 = DeviceDs18b20GetTemp((rt_base_t)sensor->config.intf.user_data);
    data->data.temp = temp_x10;
    data->timestamp = rt_sensor_get_ts();
  }
  return 1;
}

static rt_size_t DeviceDs18b20FetchData(struct rt_sensor_device *sensor, void *buf, rt_size_t len) {
  RT_ASSERT(buf);

  if(sensor->config.mode == RT_SENSOR_MODE_POLLING) {
    return _DeviceDs18b20PollingGetData(sensor, buf);
  } else {
    return 0;
  }
}

static rt_err_t DeviceDs18b20Control(struct rt_sensor_device *sensor, int cmd, void *args) {
  return RT_EOK;
}

static struct rt_sensor_ops sensor_ops = {
  DeviceDs18b20FetchData,
  DeviceDs18b20Control
};

int rt_hw_device_ds18b20_init(const char *name, struct rt_sensor_config *cfg) {
  rt_int8_t result;
  rt_sensor_t sensor_temp = RT_NULL;

  if(!DeviceDs18b20Init((rt_base_t)cfg->intf.user_data)) {

    sensor_temp = rt_calloc(1, sizeof(struct rt_sensor_device));
    if(sensor_temp == RT_NULL) {
      return RT_ERROR;
    }

    sensor_temp->info.type        = RT_SENSOR_CLASS_TEMP;
    sensor_temp->info.vendor      = RT_SENSOR_VENDOR_DALLAS;
    sensor_temp->info.model       = "ds18b20";
    sensor_temp->info.unit        = RT_SENSOR_UNIT_DCELSIUS;
    sensor_temp->info.intf_type   = RT_SENSOR_INTF_ONEWIRE;
    sensor_temp->info.range_max   = SENSOR_TEMP_RANGE_MAX;
    sensor_temp->info.range_min   = SENSOR_TEMP_RANGE_MIN;
    sensor_temp->info.period_min  = 5;

    rt_memcpy(&sensor_temp->config, cfg, sizeof(struct rt_sensor_config));
    sensor_temp->ops = &sensor_ops;

    result = rt_hw_sensor_register(sensor_temp, name, RT_DEVICE_FLAG_RDONLY, RT_NULL);
    if(result != RT_EOK) {
      LOG_E("device register err code: %d", result);
      goto __exit;
    }
  }else {
    LOG_E("DS18B20 init failed! Please check the connection!");
    goto __exit;
  }
  return RT_EOK;
  
__exit:
    if (sensor_temp)
        rt_free(sensor_temp);
    return -RT_ERROR;  
}



