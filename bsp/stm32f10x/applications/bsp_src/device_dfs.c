#include "device_dfs.h"


static int DeviceDfsMount(DeviceDfsData *device_dfs_data) {
  return dfs_mount(device_dfs_data->device_name_, device_dfs_data->path_, device_dfs_data->filesystemtype_, device_dfs_data->rwflag_, device_dfs_data->data_);
}

static int DeviceDfsopen(DeviceDfsData *device_dfs_data, const char *file, int flags, ...) {
  return open(file, flags);
}

static int DeviceDfsclose(DeviceDfsData *device_dfs_data) {
  return close(device_dfs_data->fd_);
}

static void DeviceDfsWrite(DeviceDfsData *device_dfs_data, char *file, char*str, int str_size) {
  int fd = 0;
  
  //读写 创建模式打开
  fd = open(file, O_WRONLY | O_CREAT);
  if(fd >= 0) {
    write(fd, str, str_size);
    close(fd);
    rt_kprintf("Write done ! \r\n");
  }
}

static int DeviceDfsRead(DeviceDfsData *device_dfs_data, char *file, char*str, int str_size) {
  int fd = 0;
  
  fd = open(file, O_RDONLY);
  if(fd >= 0) {
    device_dfs_data->real_read_size_ = read(fd, str, str_size);
    close(fd);
    if(device_dfs_data->real_read_size_ > 0) {
      rt_kprintf("read size = %d \r\n", device_dfs_data->real_read_size_);
      return 1;
    } else if(device_dfs_data->real_read_size_ == 0) {
      rt_kprintf("read data has reached the end of the file or there is no data to read \r\n", device_dfs_data->real_read_size_);
      return 0;
    } else {
      rt_kprintf("read : %s faild ! \r\n", file);
      return -1;
    }
  } else {
    rt_kprintf("open : %s faild ! \r\n", file);
    return -1;
  }
    
}

static int DeviceDfsRename(const char *old_name, const char *new_name) {
  return rename(old_name, new_name);
}

static int DeviceDfsStat(DeviceDfsData *device_dfs_data, const char *file) {
  return stat(file, &device_dfs_data->stat_);
}

static int DeviceDfsunLink(const char *pathname) {
  return unlink(pathname);
}

static int DeviceDfsFsync(int fildes) {
  return fsync(fildes);
}

static int DeviceDfsStatfs(DeviceDfsData *device_dfs_data, const char *file) {
  return statfs(file, &device_dfs_data->statfs_);
}

void DeviceDfsInit(DeviceDfs *c_this) {
  memset(c_this, 0, sizeof(DeviceDfs));
  
  c_this->dfs_mount = DeviceDfsMount;
  c_this->open      = DeviceDfsopen;
  c_this->close     = DeviceDfsclose;
  c_this->write     = DeviceDfsWrite;
  c_this->read      = DeviceDfsRead;
  c_this->rename    = DeviceDfsRename;
  c_this->stat      = DeviceDfsStat;
  c_this->unlink    = DeviceDfsunLink;
  c_this->fsync     = DeviceDfsFsync;
  c_this->statfs      = DeviceDfsStatfs;
}


