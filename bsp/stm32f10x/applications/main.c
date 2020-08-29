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

#define FILE               "/test.txt"
#define STR                "asdfghjkl"

#define DEVICE_DFS_NAME         "W25Q128"
#define DEVICE_DFS_PATH         "/"
#define FILE_SYSTEM_TYPE        "elm"


DeviceDfs device_dfs;    


static void DeviceDfsDemo(void) {
  DeviceDfsInit(&device_dfs);
  
  device_dfs.device_dfs_data_.device_name_    = DEVICE_DFS_NAME;
  device_dfs.device_dfs_data_.path_           = DEVICE_DFS_PATH;
  device_dfs.device_dfs_data_.filesystemtype_ = FILE_SYSTEM_TYPE;
  device_dfs.device_dfs_data_.rwflag_         = 0;
  device_dfs.device_dfs_data_.data_           = 0;

  if(device_dfs.dfs_mount(&device_dfs.device_dfs_data_) == 0) {
    rt_kprintf("W25Q128 mount to / success \r\n");
  } else {
      rt_kprintf("W25Q128 mount to / failed \r\n");
  }
  
  //写文件
  device_dfs.write(&device_dfs.device_dfs_data_, FILE, STR, sizeof(STR));
  
  //读文件
  device_dfs.read(&device_dfs.device_dfs_data_, FILE, device_dfs.device_dfs_data_.buf_, sizeof(device_dfs.device_dfs_data_.buf_));
  rt_kprintf("read : %s", device_dfs.device_dfs_data_.buf_);
}

int main(void)
{
  DeviceDfsDemo();


  return RT_EOK;
}
