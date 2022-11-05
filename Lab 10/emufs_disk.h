#include <sys/types.h>

#define BLOCKSIZE 256
#define MAX_BLOCKS 64 	// This is superblock(1) + metadata(1) + data(40)
#define MAX_FILE_SIZE 4 // In Blocks
#define MAX_INODES 32 

#define UNUSED 0
#define USED 1
#define MAGIC_NUMBER 6763

#define EMUFS_NON_ENCRYPTED 0
#define EMUFS_ENCRYPTED 1

/* ------------------- In-Disk objects ------------------- */
struct superblock_t
{
	int magic_number;                   
	char device_name[20];	            // name of the device
	int disk_size;		                // size of the device in blocks
	int fs_number;		                // File system number
				                        // -1: No filesystem exists
				                        //  0: emufs not-encrypted
				                        //  1: emufs encrypted
	char used_inodes;					// number of inodes in use
	char used_blocks;					// number of blocks in use
	char inode_bitmap[MAX_INODES];      // Bitmap of Inodes
                                        // 0 = free inode
                                        // 1 = allocated
	char block_bitmap[MAX_BLOCKS];    	// Bitmap of blocks
				    					// 0 = free block
				    					// 1 = allocated
};

struct inode_t		// 16 bytes
{
	char name[8];		    	// name of the file
    char type;                  // type of entity
                                // 0 = file
                                // 1 = directory
    char parent;                // inode of parent directory
	u_int16_t size;				// size of the file in bytes
	char mappings[4];		    // array of allocated blocks
			            		// mappings[i] = -1 : block is not allocated.
	                            // mappings[i] > 0   : block number
};

struct metadata_t	// 256 bytes
{	// Array of inodes.
	// Stored in metadata block
	struct inode_t inodes[BLOCKSIZE/16];		
};


/* ------------------- In-Memory objects ------------------- */
struct mount_t
{
	int device_fd;		        // Device number / File descriptor of opened file
					            //  <= 0: Unused mount point
					            //   > 0: file descriptor of the file used to emulate the disk
	char device_name[20]; 	    // device name / emulated file name
	int fs_number;              // File system number
    int key;                    // encryption key
};

/*--------Device--------------*/
int closedevice_(int mount_point);
void update_mount(int mount_point, int fs_number);

/*-----------FILE SYSTEM API------------*/
void read_superblock(int mount_point, struct superblock_t *superblock);
void write_superblock(int mount_point, struct superblock_t *superblock);

int alloc_inode(int mount_point);
void free_inode(int mount_point, int inodenum);
void read_inode(int mount_point, int inodenum, struct inode_t *inodeptr);
void write_inode(int mount_point, int inodenum, struct inode_t *inodeptr);

int alloc_datablock(int mount_point);
void free_datablock(int mount_point, int blocknum);
void read_datablock(int mount_point, int blocknum, char *buf);
void write_datablock(int mount_point, int blocknum, char *buf);
