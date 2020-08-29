/*
 * Copyright (c) 2006-2018, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2018-11-06     SummerGift   first version
 */
#include "main.h"


/* defined the LED0 pin: PB5 */
#define LED0_PIN    GET_PIN(B, 5)

int main(void)
{
  if(dfs_mount("W25Q128", "/", "elm", 0, 0) == 0) {
    rt_kprintf("W25Q128 mount to / success \r\n");
  } else {
      rt_kprintf("W25Q128 mount to / failed \r\n");
  }

  return RT_EOK;
}
