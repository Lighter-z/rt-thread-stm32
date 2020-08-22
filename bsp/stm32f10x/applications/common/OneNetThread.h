#ifndef __ONENET_THREAD_H__
#define __ONENET_THREAD_H__

#include <rtthread.h>
#include <rtdbg.h>


//#define ONENET_THREAD_STACK_SIZE       (1024  * 2)                          //线程栈大小
//#define ONENET_THREAD_PRIORITY         (RT_THREAD_PRIORITY_MAX / 3 - 1)     //优先级
//#define ONENET_THREAD_TIMESLICE        5                                    //时间片
#define ONENET_THREAD_STACK_SIZE       (1024  * 2)                          //线程栈大小
#define ONENET_THREAD_PRIORITY         (RT_THREAD_PRIORITY_MAX / 2 - 2)     //优先级
#define ONENET_THREAD_TIMESLICE        20                                    //时间片

#define DS18B20_GET_DATA_EVENT (1 << 3)
#define BH1750_GET_DATA_EVENT  (1 << 5)

typedef struct {
  
  rt_thread_t onenet_thread_;
  rt_thread_t onenet_mqtt_init_thread_;         /* mqtt初始化的线程的句柄 */
  
  rt_sem_t mqqt_init_sem_;
  rt_event_t recvdata_event_;
  
}OneNet_Thread;

extern OneNet_Thread onenet_thread;

void OneNetUploadData(void);

#endif
