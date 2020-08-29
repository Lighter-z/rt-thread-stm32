#include <rtdevice.h>
#include "spi_flash_sfud.h"
#include "drv_spi.h"

#define SPI_NAME                       "W25Q128"
#define SPI_BUS_NAME                   "spi2"
#define SPI_W25Q128_DEVICE_NAME        "spi20"
 
#define GPIO_X                         GPIOB
#define CS_PIN                         GPIO_PIN_12    // CS ½Å£ºPB12


static int rt_hw_spi_flash_with_sfud_init(void) {

  rt_hw_spi_device_attach(SPI_BUS_NAME, SPI_W25Q128_DEVICE_NAME, GPIO_X, CS_PIN); 

  if (RT_NULL == rt_sfud_flash_probe(SPI_NAME, SPI_W25Q128_DEVICE_NAME))
  {
    rt_kprintf("%s rt_sfud_flash_probe error \r\n", SPI_NAME);
    return RT_ERROR;
  } else {
    rt_kprintf("%s rt_sfud_flash_probe success \r\n", SPI_NAME);
  }

  return RT_EOK;
}

INIT_COMPONENT_EXPORT(rt_hw_spi_flash_with_sfud_init);

