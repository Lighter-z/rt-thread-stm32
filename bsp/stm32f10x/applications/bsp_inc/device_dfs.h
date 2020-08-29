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
  
  int fd_;                         //文件描述符
  struct stat stat_;               //存放文件信息的结构体
  struct statfs statfs_;           //文件系统相关信息
  
  char buf_[BUF_SIZE];
  int real_read_size_;             //真实读取到的大小
  
}DeviceDfsData;

typedef struct {
  
  DeviceDfsData device_dfs_data_;
  
  int (*dfs_mount)(DeviceDfsData *device_dfs_data);
  
  int (*open)(DeviceDfsData *device_dfs_data, const char *file, int flags, ...);
  int (*close)(DeviceDfsData *device_dfs_data);
  
  //内部进行打开文件，关闭文件的操作
  void(*write)(DeviceDfsData *device_dfs_data, char *file, char*str, int str_size);     
  int(*read)(DeviceDfsData *device_dfs_data, char *file, char*str, int str_size);
  
  int (*rename)(const char *old_name, const char *new_name);           //文件重命名
  int (*stat)(DeviceDfsData *device_dfs_data, const char *file);       //获取文件状态
  int (*unlink)(const char *pathname);                                 //删除文件
  int (*fsync)(int fildes);                                            //同步文件数据到存储设备
  
  int (*statfs)(DeviceDfsData *device_dfs_data, const char *file);     //查询文件系统相关信息
  
  
}DeviceDfs;

void DeviceDfsInit(DeviceDfs *c_this);

#endif
