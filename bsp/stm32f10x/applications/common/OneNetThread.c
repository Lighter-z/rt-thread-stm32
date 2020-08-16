#include "OneNetThread.h"

#include <stdlib.h>
#include <onenet.h>

extern rt_sem_t mqqt_init_sem;

OneNet_Thread onenet_thread;

static void OneNetUploadEntry(void *parameter) {
  int value = 0;

  rt_sem_take(mqqt_init_sem, RT_WAITING_FOREVER);
  /* 后面用不到这个信号量了，把它删除了，回收资源 */
  rt_sem_delete(mqqt_init_sem);
  while (1)
  {
      value = rand() % 100;

      if (onenet_mqtt_upload_digit("temperature", value) < 0)
      {
          LOG_E("upload has an error, stop uploading");
          break;
      }
      else
      {
          LOG_D("buffer : {\"temperature\":%d}", value);
      }

      rt_thread_delay(rt_tick_from_millisecond(5 * 1000));
  }
}

//onenet 回调响应函数
static void OneNetCmdRspCb(uint8_t *recv_data, size_t recv_size, uint8_t **resp_data, size_t *resp_size) {
  
}

void OneNetUploadCycle(void) {
  
  /* 设置 onenet 回调响应函数 */
  onenet_set_cmd_rsp_cb(OneNetCmdRspCb);
  
  /* 传创建线程 */
  onenet_thread.onenet_thread_ = rt_thread_create("OneNet_Thread", 
                                                  OneNetUploadEntry, RT_NULL,
                                                  ONENET_THREAD_STACK_SIZE,
                                                  ONENET_THREAD_PRIORITY,
                                                  ONENET_THREAD_TIMESLICE);
  if(onenet_thread.onenet_thread_ != RT_NULL) {
    rt_thread_startup(onenet_thread.onenet_thread_);
  }
}

MSH_CMD_EXPORT(OneNetUploadCycle, Upload Data To OneNet);

