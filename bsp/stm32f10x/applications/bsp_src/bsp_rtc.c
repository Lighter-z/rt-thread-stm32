#include "bsp_rtc.h"

/*
 * �����嵥������һ�� RTC �豸ʹ������
 * ���̵����� rtc_sample ��������ն�
 * ������ø�ʽ��rtc_sample
 * �����ܣ�����RTC�豸�����ں�ʱ�䣬��ʱһ��ʱ����ȡ��ǰʱ�䲢��ӡ��ʾ��
*/

#include <rtthread.h>
#include <rtdevice.h>

static int rtc_sample(int argc, char *argv[])
{
    rt_err_t ret = RT_EOK;
    time_t now;

    /* �������� */
    ret = set_date(2018, 12, 3);
    if (ret != RT_EOK)
    {
        rt_kprintf("set RTC date failed\n");
        return ret;
    }

    /* ����ʱ�� */
    ret = set_time(11, 15, 50);
    if (ret != RT_EOK)
    {
        rt_kprintf("set RTC time failed\n");
        return ret;
    }

    /* ��ʱ3�� */
    rt_thread_mdelay(3000);

    /* ��ȡʱ�� */
    now = time(RT_NULL);
    rt_kprintf("%s\n", ctime(&now));

    return ret;
}
/* ������ msh �����б��� */
MSH_CMD_EXPORT(rtc_sample, rtc sample);
