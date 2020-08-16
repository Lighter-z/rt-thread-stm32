#ifndef __ONENET_THREAD_H__
#define __ONENET_THREAD_H__

#include <rtthread.h>
#include <rtdbg.h>


#define ONENET_THREAD_STACK_SIZE       (1024  * 2)                          //�߳�ջ��С
#define ONENET_THREAD_PRIORITY         (RT_THREAD_PRIORITY_MAX / 3 - 1)     //���ȼ�
#define ONENET_THREAD_TIMESLICE        5                                    //ʱ��Ƭ

typedef struct {
  
  rt_thread_t onenet_thread_;
  
}OneNet_Thread;

void OneNetUploadCycle(void);

#endif
