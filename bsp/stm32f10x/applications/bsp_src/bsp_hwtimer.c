//#include "bsp_hwtimer.h"

//static rt_sem_t bsp_sem = RT_NULL;  /* 定义信号量 */
///***************************************************************
//*   函数：  static rt_err_t bsp_hwtimer_cb(rt_device_t dev, rt_size_t size)
//*   参数：  dev 硬件定时器设备句柄
//*           size 长度
//*   返回值：0  
//*   功能：  硬件定时器超时回调函数，释放信号量
//*****************************************************************/
//static rt_err_t bsp_hwtimer_cb(rt_device_t dev, rt_size_t size)
//{
//    /* 定时时间到释放信号量 */
//	if(rt_sem_release(bsp_sem) == RT_EOK)
//    {
//        rt_kprintf("this is hwtimer timeout callback function!, semaphore have released!\n");     /* 打印回调信息 */
//        rt_kprintf("tick is %d !\n", rt_tick_get());                                              /* 打印系统tick值 */
//    }

//    return RT_EOK;
//}

///***************************************************************
//*   函数：  static rt_uint8_t bsp_hwtimer_sample(int argc, char *argv[])
//*   参数：  int argc, char *argv[]
//*   返回值：无  
//*   功能：  创建信号量，设置蜂鸣器引脚默认模式、查找硬件定时器设备，设置定时器相关信息
//*****************************************************************/
//static rt_uint8_t bsp_hwtimer_sample(int argc, char *argv[])
//{
//    static rt_device_t bsp_hwtim_dev = RT_NULL;     /* 定义硬件定时器设备控制块 */
//    static rt_hwtimerval_t timeout_s;               /* 定义硬件定时器超时时间结构体 */

//    static rt_thread_t bsp_sem_tid = RT_NULL;       /* 定义信号量控制块 */

//    rt_err_t ret = RT_EOK;
//    rt_hwtimer_mode_t hwmode = HWTIMER_MODE_PERIOD; /* 定义硬件定时器定时模式：周期定时 */
//    rt_uint32_t hwfreq = 10000;                     /* 定义硬件定时器计数频率：10000Hz=10KHz */

//    /* 设置蜂鸣器引脚默认模式，低电平不响 */
//    rt_pin_mode(BEEP_PIN, PIN_MODE_OUTPUT);
//    rt_pin_write(BEEP_PIN, PIN_LOW);

//    /* 创建信号量，初始值为0，先进先出原则 */
//    bsp_sem = rt_sem_create("bsp_sem", 0, RT_IPC_FLAG_FIFO);

//    /* 查找硬件定时器设备 */
//    bsp_hwtim_dev = rt_device_find(HWTIMER_DEV_NAME);
//    if (bsp_hwtim_dev == RT_NULL)
//    {
//        rt_kprintf("hwtimer device sample run failed! can't find %s device!\n", HWTIMER_DEV_NAME);
//        return RT_ERROR;
//    }

//    /* 以读写方式打开硬件定时器 */
//    ret = rt_device_open(bsp_hwtim_dev, RT_DEVICE_OFLAG_RDWR);
//    if (ret != RT_EOK)
//    {
//        rt_kprintf("open %s hwtimer failed!\n", HWTIMER_DEV_NAME);
//        return RT_ERROR;
//    }

//    /* 设置硬件定时器超时回调函数 */
//    rt_device_set_rx_indicate(bsp_hwtim_dev, bsp_hwtimer_cb);
//	
//    
//    /* 设置硬件定时器的定时模式为周期定时 */
//    ret = rt_device_control(bsp_hwtim_dev, HWTIMER_CTRL_MODE_SET, &hwmode);
//    if (ret != RT_EOK)
//    {
//        rt_kprintf("set %s hwtimer mode failed!\n", HWTIMER_DEV_NAME);
//        return RT_ERROR;
//    }

//    /* 设置硬件定时器的计数频率为10000Hz */
//    ret = rt_device_control(bsp_hwtim_dev, HWTIMER_CTRL_FREQ_SET, &hwfreq);
//    if (ret != RT_EOK)
//    {
//        rt_kprintf("set %s hwtimer frequency failed!\n", HWTIMER_DEV_NAME);
//        return RT_ERROR;
//    }

//    timeout_s.sec = 5;
//    timeout_s.usec = 0;

//    /* 设置硬件定时器的超时时间为5s，并启动定时器 */
//    if(rt_device_write(bsp_hwtim_dev, 0, &timeout_s, sizeof(timeout_s)) != sizeof(timeout_s))
//    {
//        rt_kprintf("set %s timeout value failed!\n", HWTIMER_DEV_NAME);
//        return RT_ERROR;
//    }

//    /* 延时3500ms后获取定时器当前值，并打印出来 */    
//    rt_thread_mdelay(3500);
//    rt_device_read(bsp_hwtim_dev, 0, &timeout_s, sizeof(timeout_s));
//    rt_kprintf("READ : Sec = %d, Usec = %d \n", timeout_s.sec, timeout_s.usec);
//	
//    /* 创建信号量线程 */
//    bsp_sem_tid = rt_thread_create("bsp_sem_tid", bsp_sem_thread_entry, RT_NULL,
//                                    512, BSP_SEM_PRIORITY, 10);

//    /* 启动信号量线程 */
//    if (bsp_sem_tid != RT_NULL)
//    {
//        rt_thread_startup(bsp_sem_tid);
//    }


//    return RT_EOK;  
//}

///* 导出bsp_hwtimer_sample命令至FinSH终端 */
//MSH_CMD_EXPORT(bsp_hwtimer_sample, hwtimer sample);

///* 信号量线程 */
//static void bsp_sem_thread_entry(void *parameter)
//{
//    while (1)
//    {
//        /* 获取到信号量后蜂鸣器发出“滴”声 */
//        if(rt_sem_take(bsp_sem, RT_WAITING_FOREVER) == RT_EOK)
//        {
//            rt_pin_write(BEEP_PIN, PIN_HIGH);
//            rt_thread_mdelay(200);
//            rt_pin_write(BEEP_PIN, PIN_LOW);
//        }
//    }  
//}

#include "bsp_hwtimer.h"

//信号量线程
static rt_thread_t bsp_sem_tid = RT_NULL;
//信号量
static rt_sem_t bsp_sem = RT_NULL;

/***************************************************************
*   函数：  static rt_err_t TimerOutCb(rt_device_t dev, rt_size_t size)
*   参数：  dev 硬件定时器设备句柄
*           size 长度
*   返回值：RT_EOK
*   功能：  硬件定时器超时回调函数，释放信号量
*****************************************************************/
static rt_err_t TimerOutCb(rt_device_t dev, rt_size_t size) {
   
  if(rt_sem_release(bsp_sem) == RT_EOK) {
    rt_kprintf("this is hwtimer timeout callback fucntion!\n");
    rt_kprintf("tick is :%d !\n", rt_tick_get());
  }

  return RT_EOK;  
}

/***************************************************************
*   函数：  static void bsp_sem_thread_entry(void *parameter)
*   参数：  parameter 线程传递参数
*            
*   返回值：  
*   功能：  获取信号量，蜂鸣器发出声音
*****************************************************************/
static void bsp_sem_thread_entry(void *parameter) {
  while(1) {
    if(rt_sem_take(bsp_sem, RT_WAITING_FOREVER) == RT_EOK) {
      rt_pin_write(BEEP_PIN, PIN_HIGH);
      rt_thread_mdelay(200);
      rt_pin_write(BEEP_PIN, PIN_LOW);
    }
  }
}

/***************************************************************
*   函数：  static void BeepInit(void)
*   参数：   
*            
*   返回值： 
*   功能：  初始化蜂鸣器，设置默认电平为低电平
*****************************************************************/
static void BeepInit(void) {
  rt_pin_mode(BEEP_PIN, PIN_MODE_OUTPUT);
  rt_pin_write(BEEP_PIN, PIN_LOW);
}

/***************************************************************
*   函数：  static int BspHwtimerSample(int argc, char *argv[])
*   参数：  int argc, char *argv[]
*            
*   返回值： 
*   功能：  1、初始化蜂鸣器
            2、查找定时器设备，设置定时器相关参数
            3、创建信号量线程
*****************************************************************/
static int BspHwtimerSample(int argc, char *argv[]) {
  rt_err_t ret = RT_EOK;
  static rt_device_t bsp_hwtimer_dev = RT_NULL;                 //定义硬件定时器设备控制块
  static rt_hwtimerval_t timeout_s;                             //定义硬件定时器超时时间结构体
  rt_hwtimer_mode_t bsp_hwtimer_mode = HWTIMER_MODE_PERIOD;     //定义硬件定时器模式：周期定时
  rt_uint32_t hw_freq = 10000;                                  //定义硬件定时器频率：10000HZ
  
  //初始化蜂鸣器
  BeepInit();
  
  //查找硬件定时器设备
  bsp_hwtimer_dev = rt_device_find(HWTIMER_DEV_NAME);
  if(bsp_hwtimer_dev == RT_NULL) {
    rt_kprintf("hwtimer device sample run failed! can't find %s device!\n", HWTIMER_DEV_NAME);
    return RT_ERROR;
  }
  
  //以读写模式打开硬件定时器
  ret = rt_device_open(bsp_hwtimer_dev, RT_DEVICE_OFLAG_RDWR);
  if(ret != RT_EOK) {
    rt_kprintf("set %s hwtimer mode failed!\n", HWTIMER_DEV_NAME);
    return RT_ERROR;
  }
  
  //设置超时回调函数
  rt_device_set_rx_indicate(bsp_hwtimer_dev, TimerOutCb);
  
  //设置硬件定时器的定时模式为周期定时
  ret = rt_device_control(bsp_hwtimer_dev, HWTIMER_CTRL_MODE_SET, &bsp_hwtimer_mode);
  if(ret != RT_EOK) {
    rt_kprintf("set %s hwtimer mode failed!\n", HWTIMER_DEV_NAME);
    return RT_ERROR;
  }
  
  /* 设置硬件定时器的计数频率为10000Hz */
  ret = rt_device_control(bsp_hwtimer_dev, HWTIMER_CTRL_FREQ_SET, &hw_freq);
  if (ret != RT_EOK)
  {
      rt_kprintf("set %s hwtimer frequency failed!\n", HWTIMER_DEV_NAME);
      return RT_ERROR;
  }
  
  timeout_s.sec = 5;
  timeout_s.usec = 0;
  //设置硬件定时器的超时时间为5s，并启动定时器
  if(rt_device_write(bsp_hwtimer_dev, 0, &timeout_s, sizeof(timeout_s)) != sizeof(timeout_s)) {
    rt_kprintf("set %s timeout value failed!\n", HWTIMER_DEV_NAME);
    return RT_ERROR;
  }
  /* 延时3500ms */
  rt_thread_delay(3500);
  
  //读取定时器当前值
  rt_device_read(bsp_hwtimer_dev, 0, &timeout_s, sizeof(timeout_s));
  //定时器打印 Read: sec = 3, usec = 499399 
  rt_kprintf("Read: sec = %d, usec = %d \r\n", timeout_s.sec, timeout_s.usec);
  
  //创建信号量，初始值为0
  bsp_sem = rt_sem_create("bsp_sem", 0, RT_IPC_FLAG_FIFO);
  
  //创建信号量线程
  bsp_sem_tid = rt_thread_create("bsp_sem_tid", bsp_sem_thread_entry, RT_NULL, BSP_SEM_STACK_SIZE, BSP_SEM_PRIORITY, BSP_SEM_TICK);
  
  //启动信号量线程
  if(bsp_sem_tid != RT_NULL) {
    rt_thread_startup(bsp_sem_tid);
  }
  
  return ret;
}

/* 导出BspHwtimerSample命令至FinSH终端 */
MSH_CMD_EXPORT(BspHwtimerSample, hwtimer_sample);



