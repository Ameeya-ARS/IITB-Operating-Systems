#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/types.h>
#include <unistd.h>
#include <time.h>
#include <string.h>

#define MAX_FILE_HANDLES 20
#define MAX_DIR_HANDLES 20
#define MAX_MOUNT_POINTS 10
#define MAX_ENTITY_NAME 8

/*-----------DEVICE------------*/
int opendevice(char *device_name, int size);
int closedevice(int mount_point);
void mount_dump(void);

/*-----------FILE SYSTEM API------------*/
int create_file_system(int mount_point, int fs_number);
void fsdump(int mount_point);

int open_root(int mount_point);
int change_dir(int dir_handle, char* path);

int open_file(int dir_handle, char* path);

int emufs_create(int dir_handle, char* name, int type);
int emufs_delete(int dir_handle, char* path);
void emufs_close(int handle, int type);

int emufs_read(int file_handle, char* buf, int size);
int emufs_write(int file_handle, char* buf, int size);
int emufs_seek(int file_handle, int nseek);
