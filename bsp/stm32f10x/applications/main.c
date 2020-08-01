/*
 * Copyright (c) 2006-2018, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2018-11-06     SummerGift   first version
 */

#include <rtthread.h>
#include <rtdevice.h>
#include <board.h>

/* defined the LED0 pin: PB5 */
#define LED0_PIN    GET_PIN(B, 5)

//beep pin: PB8
#define BEEP_PIN    GET_PIN(B, 8)

//key0 pin: PE4
#define KEY0_PIN    GET_PIN(E, 4)
//key2 pin: PE2
#define KEY2_PIN    GET_PIN(E, 2)


static void BeepOn(void *args) {
	rt_pin_write(BEEP_PIN, PIN_LOW);
}

static void BeepOff(void *args) {
	rt_pin_write(BEEP_PIN, PIN_HIGH);
}

static void BeepInit(void) {
	rt_pin_mode(BEEP_PIN, PIN_MODE_OUTPUT );
	rt_pin_write(BEEP_PIN, PIN_LOW);
}

//#define PIN_IRQ_MODE_RISING 0x00         /* 上升沿触发 */
//#define PIN_IRQ_MODE_FALLING 0x01        /* 下降沿触发 */
//#define PIN_IRQ_MODE_RISING_FALLING 0x02 /* 边沿触发（上升沿和下降沿都触发）*/
//#define PIN_IRQ_MODE_HIGH_LEVEL 0x03     /* 高电平触发 */
//#define PIN_IRQ_MODE_LOW_LEVEL 0x04      /* 低电平触发 */

static void KeyInit(void) {
  //按键初始化为上拉输入模式
  rt_pin_mode(KEY0_PIN, PIN_MODE_INPUT_PULLUP );
  rt_pin_mode(KEY2_PIN, PIN_MODE_INPUT_PULLUP );
  //绑定按键中断，下降沿触发
  rt_pin_attach_irq(KEY0_PIN, PIN_IRQ_MODE_FALLING, BeepOn, RT_NULL);
  rt_pin_irq_enable(KEY0_PIN, PIN_IRQ_ENABLE);
  
  rt_pin_attach_irq(KEY2_PIN, PIN_IRQ_MODE_FALLING, BeepOff, RT_NULL);
  rt_pin_irq_enable(KEY2_PIN, PIN_IRQ_ENABLE);
  
}


int main(void)
{
  int count = 1;
  /* set LED0 pin mode to output */
  rt_pin_mode(LED0_PIN, PIN_MODE_OUTPUT);

  BeepInit();
  
  KeyInit();

  while (count++)
  {
    rt_pin_write(LED0_PIN, PIN_HIGH);
    rt_thread_mdelay(500);
    rt_pin_write(LED0_PIN, PIN_LOW);
    rt_thread_mdelay(500);
  }

  return RT_EOK;
}
