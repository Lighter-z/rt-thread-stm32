#ifndef __ONENET_THREAD_H__
#define __ONENET_THREAD_H__

#include <rtthread.h>
#include <rtdbg.h>


#define ONENET_THREAD_STACK_SIZE       (1024  * 2)                          //线程栈大小
#define ONENET_THREAD_PRIORITY         (RT_THREAD_PRIORITY_MAX / 3 - 1)     //优先级
#define ONENET_THREAD_TIMESLICE        5                                    //时间片

typedef struct {
  
  rt_thread_t onenet_thread_;
  
}OneNet_Thread;

void OneNetUploadCycle(void);

#endif
