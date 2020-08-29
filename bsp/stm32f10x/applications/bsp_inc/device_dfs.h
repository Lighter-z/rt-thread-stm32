#ifndef __DEVICE_DFS_H__
#define __DEVICE_DFS_H__

#include <rtthread.h>
#include <dfs_posix.h>
#include <dfs_fs.h>


#define BUF_SIZE    100

typedef struct {
  
  const char *device_name_;
  const char *path_;
  const char *filesystemtype_;
  unsigned long rwflag_;
  const void *data_;
  
  int fd_;                         //�ļ�������
  struct stat stat_;               //����ļ���Ϣ�Ľṹ��
  struct statfs statfs_;           //�ļ�ϵͳ�����Ϣ
  
  char buf_[BUF_SIZE];
  int real_read_size_;             //��ʵ��ȡ���Ĵ�С
  
}DeviceDfsData;

typedef struct {
  
  DeviceDfsData device_dfs_data_;
  
  int (*dfs_mount)(DeviceDfsData *device_dfs_data);
  
  int (*open)(DeviceDfsData *device_dfs_data, const char *file, int flags, ...);
  int (*close)(DeviceDfsData *device_dfs_data);
  
  //�ڲ����д��ļ����ر��ļ��Ĳ���
  void(*write)(DeviceDfsData *device_dfs_data, char *file, char*str, int str_size);     
  int(*read)(DeviceDfsData *device_dfs_data, char *file, char*str, int str_size);
  
  int (*rename)(const char *old_name, const char *new_name);           //�ļ�������
  int (*stat)(DeviceDfsData *device_dfs_data, const char *file);       //��ȡ�ļ�״̬
  int (*unlink)(const char *pathname);                                 //ɾ���ļ�
  int (*fsync)(int fildes);                                            //ͬ���ļ����ݵ��洢�豸
  
  int (*statfs)(DeviceDfsData *device_dfs_data, const char *file);     //��ѯ�ļ�ϵͳ�����Ϣ
  
  
}DeviceDfs;

void DeviceDfsInit(DeviceDfs *c_this);

#endif
