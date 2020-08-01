#include "bsp_adc.h"

/* 定义adc设备控制块 */
static rt_adc_device_t bsp_adc_dev = RT_NULL;

/***************************************************************
*   函数：  static void bsp_adc_thread_entry(void *parameter)
*   参数：  空指针
*   返回值：无  
*   功能：  adc线程，实现adc设备通道数据读取、转换以及打印
*****************************************************************/
static void bsp_adc_thread_entry(void *parameter)
{
    volatile rt_uint32_t adc_value = 0;
    rt_uint32_t vol = 0;

    while (1)
    {
        /* 隔10s读一次adc设备所在通道数据，并进行处理 */
        rt_thread_mdelay(10000);

        adc_value = rt_adc_read(bsp_adc_dev, ADC_DEV_CHANNEL);
        rt_kprintf("the value is: %d\n", adc_value);
        vol = adc_value * REFER_VOLTAGE / CONVERT_BITS;
        rt_kprintf("the voltage is : %d.%02d\n", vol / 100, vol % 100);
    }  
}

/***************************************************************
*   函数：  static void bsp_adc_sample(int arc, char *argv[])
*   参数：  int argc, char *argv[]
*   返回值：无 
*   功能：  查找adc设备、使能adc设备、创建并启动adc线程
*****************************************************************/
static void bsp_adc_sample(int arc, char *argv[])
{
    static rt_thread_t bsp_adc_tid = RT_NULL;

    bsp_adc_dev = (rt_adc_device_t)rt_device_find(ADC_DEV_NAME);
    if (bsp_adc_dev != RT_NULL)
    {
        rt_kprintf("adc device fined!\n");
    }

    rt_adc_enable(bsp_adc_dev, ADC_DEV_CHANNEL);

    bsp_adc_tid = rt_thread_create("bsp_adc_tid", bsp_adc_thread_entry, RT_NULL, 512, BSP_ADC_PRIORITY, 10);

    if (bsp_adc_tid != RT_NULL)
    {
        rt_thread_startup(bsp_adc_tid);
    }
    
}
 
/* 导出bsp_adc_sample命令至FinSH终端 */
MSH_CMD_EXPORT(bsp_adc_sample, bsp adc device sample);
