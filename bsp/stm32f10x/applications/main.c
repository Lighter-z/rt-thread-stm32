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


int main(void)
{
  
  OneNetUploadData();
  
  return RT_EOK;
}
