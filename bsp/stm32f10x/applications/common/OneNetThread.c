#include "OneNetThread.h"

#include <stdlib.h>
#include <onenet.h>
#include "device_ds18b20.h"

/*****************************
1.使用信号量检测ds18b20的采集：device_ds18b20.ds18b20_sem_
2. 测试两组数据的上传：device_ds18b20.data_  value
3.每组数组上转之间要有时间间隔（具体看情况分析）
3.MQTT的接收超时：
  urc_recv_func函数中设置，设置规则：根据数据大小计算bfsz，然后 timeout = bfsz;
*****************************/

OneNet_Thread onenet_thread;


//onenet 回调响应函数
static void OneNetCmdRspCb(uint8_t *recv_data, size_t recv_size, uint8_t **resp_data, size_t *resp_size) {
  
}

static void OneNetUploadEntry(void *parameter) {
  int value = 0;

   /* 永久等待方式接收信号量，若收不到，该线程会一直挂起 */
  rt_sem_take(onenet_thread.mqqt_init_sem_, RT_WAITING_FOREVER);
  /* 后面用不到这个信号量了，把它删除了，回收资源 */
  rt_sem_delete(onenet_thread.mqqt_init_sem_);
  
  /* 500ms上传一次数据 */
  rt_thread_delay(rt_tick_from_millisecond(500));  

  while (1) {
    
    value = rand() % 100;
    
    rt_sem_take(device_ds18b20.ds18b20_sem_, RT_WAITING_FOREVER);
    rt_thread_delay(rt_tick_from_millisecond(300));
    //device_ds18b20.sensor_data_.timestamp
    if (onenet_mqtt_upload_digit("temperature", device_ds18b20.data_) < 0) {
        LOG_E("upload has an error, stop uploading");
        break;
    } else {
        LOG_D("buffer : {\"temperature\":%d}", device_ds18b20.data_);
    }
    rt_thread_delay(rt_tick_from_millisecond(300));
    
    if (onenet_mqtt_upload_digit("vale_test", value) < 0) {
      LOG_E("upload has an error, stop uploading");
      break;
    } else {
        LOG_D("buffer : {\"DEm\":%d}", value);
    }
  }
}


//数据上传线程
static void OneNetUploadCycle(void) {
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


static void onenet_mqtt_init_entry(void *parameter) {
  uint8_t onenet_mqtt_init_failed_times;

  /* mqtt初始化 */
  while (1) {
    if (!onenet_mqtt_init()) {
        
      /* mqtt初始化成功之后，释放信号量告知onenet_upload_data_thread线程可以上传数据了 */
      rt_sem_release(onenet_thread.mqqt_init_sem_);
      return;
    }
    rt_thread_mdelay(100);
    LOG_E("onenet mqtt init failed %d times", onenet_mqtt_init_failed_times++);
  }
}

//初始化MQTT线程
static void OneNetMqttInit(void) {
  
  onenet_thread.mqqt_init_sem_ = rt_sem_create("mqtt_sem", RT_NULL, RT_IPC_FLAG_FIFO);
  RT_ASSERT(onenet_thread.mqqt_init_sem_);
  onenet_thread.onenet_mqtt_init_thread_ = rt_thread_create("mqttinit", 
                                                            onenet_mqtt_init_entry, RT_NULL,
                                                            ONENET_THREAD_STACK_SIZE, 
                                                            ONENET_THREAD_PRIORITY,
                                                            ONENET_THREAD_TIMESLICE);
  if (onenet_thread.onenet_mqtt_init_thread_ != RT_NULL) {
    rt_thread_startup(onenet_thread.onenet_mqtt_init_thread_);
  }
}

void OneNetUploadData(void) {
  OneNetMqttInit();
  OneNetUploadCycle();
}


