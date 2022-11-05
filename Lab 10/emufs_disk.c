#include "emufs_disk.h"
#include "emufs.h"

struct mount_t mounts[MAX_MOUNT_POINTS];


/*-----------DEVICE------------*/
int writeblock(int dev_fd, int block, char* buf)
{
	/*
		* Writes the memory buffer to a block in the device

		* Return value: -1, error
						 1, success
	*/

	int ret;
	int offset;

	if(dev_fd < 0)
	{
		printf("Devices not found \n");
		return -1;
	}

	offset = block * BLOCKSIZE;
	lseek(dev_fd, offset, SEEK_SET);
	ret = write(dev_fd, buf, BLOCKSIZE);
	if(ret != BLOCKSIZE)
	{
		printf("Error: Disk write error. fd: %d. block: %d. buf: %p. ret: %d \n", dev_fd, block, buf, ret);
		return -1;
	}

	return 1;
}

int readblock(int dev_fd, int block, char * buf)
{
	/*
		* Writes a block in the device to the memory buffer

		* Return value: -1, error
						 1, success
	*/

	int ret;
	int offset;

	if(dev_fd < 0)
	{
		printf("Devices not found\n");
		return -1;
	}
	offset = block * BLOCKSIZE;
	lseek(dev_fd, offset, SEEK_SET);
	ret = read(dev_fd, buf, BLOCKSIZE);
	if(ret != BLOCKSIZE)
	{
		printf("Error: Disk read error. fd: %d. block: %d. buf: %p. ret: %d \n", dev_fd, block, buf, ret);
		return -1;
	}

	return 1;
}


/*-----------ENCRYPTION------------*/
void encrypt(int key, char* buf, int size){
	/*
		* Encrypts the buffer of size 'size' using key
	*/

	for(int i=0; i<size; i++)
		buf[i] = (buf[i]+key)%256;
}

void decrypt(int key, char* buf, int size){
	/*
		* Decrypts the buffer of size 'size' using key
	*/

	for(int i=0; i<size; i++){
		if(buf[i] < key)
			buf[i] = 256 - key + buf[i];
		else 
			buf[i] = buf[i] - key;
	}
}


/*----------MOUNT-------*/
int add_new_mount_point(int fd, char *device_name, int fs_number)
{
	/*
		* Creates a mount for the device
		* Assigns an entry in the mount devices array

		* Return value: -1,									error
						array entry index (mount point)		success
	*/

	struct mount_t* mount_point = NULL;

	for(int i=0; i<MAX_MOUNT_POINTS; i++)
		if(mounts[i].device_fd <= 0 )
		{
			mount_point = &mounts[i];
			mount_point->device_fd = fd;
			strcpy(mount_point->device_name, device_name);
			mount_point->fs_number = fs_number;

			return i;
		}

	return -1;
}


int opendevice(char* device_name, int size)
{
	/*
		* Opens a device if it exists and do some consistency checks
		* Creates a device of given size if not present
		* Assigns a mount point

		* Return value: -1, 			error
						 mount point,	success	
	*/			

	int fd;
	FILE* fp;
	char tempBuf[BLOCKSIZE];
	struct superblock_t* superblock;
	int mount_point;
	int key;

	if(!device_name || strlen(device_name) == 0)
	{
		printf("Error: Invalid device name \n");
		return -1;
	}

	if(size > MAX_BLOCKS || size < 3)
	{
		printf("Error: Invalid disk size \n");
		return -1;
	}

	superblock = (struct superblock_t*)malloc(sizeof(struct superblock_t));
	fp = fopen(device_name, "r");
	if(!fp)
	{
		//	Creating the device
		printf("[%s] Creating the disk image \n", device_name);

		superblock->fs_number =  -1; 	//	No fs in the disk
		strcpy(superblock->device_name, device_name);
		superblock->disk_size = size;
		superblock->magic_number = MAGIC_NUMBER;	

		fp = fopen(device_name, "w+");
		if(!fp)
		{
			printf("Error : Unable to create the device. \n");
			free(superblock);
			return -1;
		}
		fd = fileno(fp);

		// Make size of the disk as the total size
		// printf("Current offset: %ld \n", lseek( fd, 0, SEEK_CUR ));
		fseek(fp, size * BLOCKSIZE, SEEK_SET);
		fputc('\0', fp);
		fseek(fp, 0, SEEK_SET);

		// Allocating super block on the disk
		memcpy(tempBuf, superblock, sizeof(struct superblock_t));
		writeblock(fd, 0, tempBuf);

		printf("[%s] Disk image is successfully created \n", device_name);
	}
	else
	{
		fclose(fp);
		fd = open(device_name, O_RDWR);

		readblock(fd, 0, tempBuf);
		memcpy(superblock, tempBuf, sizeof(struct superblock_t));
		if(superblock->fs_number==EMUFS_ENCRYPTED){
			printf("Input key: ");
			scanf("%d",&key);
			decrypt(key, (char*)&(superblock->magic_number),4);
		}
		if(superblock->magic_number != MAGIC_NUMBER || superblock->disk_size < 3 || superblock->disk_size > MAX_BLOCKS)
		{
			printf("%d,%d,%d",superblock->magic_number,superblock->disk_size,superblock->disk_size);
			printf("Error: Inconsistent super block on device. \n");
			free(superblock);
			return -1;
		}
		printf("[%s] Disk opened \n", device_name);

		if(superblock->fs_number == -1)
			printf("[%s] File system found in the disk \n", device_name);
		else
			printf("[%s] File system found. fs_number: %d \n", device_name, superblock->fs_number);
		
	}	

	mount_point = add_new_mount_point(fd, device_name, superblock->fs_number);
	if(superblock->fs_number==1)
		mounts[mount_point].key=key;

	printf("[%s] Disk successfully mounted \n", device_name);
	free(superblock);

	return mount_point;	
}


int closedevice_(int mount_point)
{
	/*
		* Closes a device

		* Return value: -1, error
						 1, success
	*/

	char device_name[20];

	if(mounts[mount_point].device_fd < 0)
	{
		printf("Error: Devices not found\n");
		return -1;
	}

	strcpy(device_name, mounts[mount_point].device_name);
	close(mounts[mount_point].device_fd);

	mounts[mount_point].device_fd = -1;
	strcpy(mounts[mount_point].device_name, "\0");
	mounts[mount_point].fs_number = -1;

	printf("[%s] Device closed \n", device_name);
	return 1;
}

void update_mount(int mount_point, int fs_number){
	/*
		* Update the mount point with the file system number
		* Prompts for key if its an encrypted file system
	*/

	int key;
	mounts[mount_point].fs_number = fs_number;
	if(fs_number==1){
		printf("Input key: ");
		scanf("%d",&key);
		mounts[mount_point].key=key;
	}
	/*
		update the fstype in the mount and prompt the user for key if its an encryted system
	*/
}

void mount_dump(void)
{
	/*
		* Prints summary of mount points
	*/

	struct mount_t* mount_point;

	printf("\n%-12s %-20s %-15s %-10s %-20s \n", "MOUNT-POINT", "DEVICE-NAME", "DEVICE-NUMBER", "FS-NUMBER", "FS-NAME");
	for(int i=0; i< MAX_MOUNT_POINTS; i++)
	{
		mount_point = mounts + i;
		if(mount_point->device_fd <= 0)
			continue;

		if(mount_point->device_fd > 0)
			printf("%-12d %-20s %-15d %-10d %-20s\n", 
					i, mount_point->device_name, mount_point->device_fd, mount_point->fs_number, 
					mount_point->fs_number == EMUFS_NON_ENCRYPTED ? "emufs non-encrypted" : (mount_point->fs_number == EMUFS_ENCRYPTED ? "emufs encrypted" : "Unknown file system"));
	}
}

void read_superblock(int mount_point, struct superblock_t *superblock){
	/*	
		* Reads the superblock of the device
		* If its an encrypted system, decrypts the magic number after reading
	*/

	char tempBuf[BLOCKSIZE];
	readblock(mounts[mount_point].device_fd, 0, tempBuf);
	if(mounts[mount_point].fs_number==1)
		decrypt(mounts[mount_point].key, tempBuf, 4);
	memcpy(superblock, tempBuf, sizeof(struct superblock_t));
}

void write_superblock(int mount_point, struct superblock_t *superblock){
	/*
		* Updates the superblock of the device
		* If its an encrypted system, encrypts the magic number before writing
	*/

	char tempBuf[BLOCKSIZE];
	memcpy(tempBuf, superblock, sizeof(struct superblock_t));
	if(mounts[mount_point].fs_number==1)
		encrypt(mounts[mount_point].key, tempBuf, 4);
	writeblock(mounts[mount_point].device_fd, 0, tempBuf);
}

int alloc_inode(int mount_point){
	/*
		* Checks if there are any free inodes
		* Update the inode bitmap and used_inodes 
		
		* Return value: -1,				error
						 inode number, 	success
	*/

	struct superblock_t superblock;
	read_superblock(mount_point, &superblock);
	if(superblock.used_inodes == MAX_INODES)
		return -1;
	for(int i=0; i<MAX_INODES; i++){
		if(superblock.inode_bitmap[i]==0){
			superblock.inode_bitmap[i]=1;
			superblock.used_inodes++;
			write_superblock(mount_point, &superblock);
			return i;
		}
	}
}

void free_inode(int mount_point, int inodenum){
	/*
		* Updates the inode bitmap and used_inodes in the superblock
	*/

	struct superblock_t superblock;
	read_superblock(mount_point, &superblock);
	superblock.inode_bitmap[inodenum] = 0;
	superblock.used_inodes--;
	write_superblock(mount_point, &superblock);
}

void read_inode(int mount_point, int inodenum, struct inode_t *inodeptr){
	/*
		* Reads the metadata in block 2 or 3
		* Decrypts the metadata block if its an encrypted system
		* Reads the inode entry from the metadata block into the memory buffer
	*/

	struct metadata_t metadata;
	int d=inodenum/16, r=inodenum%16;
	readblock(mounts[mount_point].device_fd, 1+d, (char*)&metadata);
	if(mounts[mount_point].fs_number==1)
		decrypt(mounts[mount_point].key, (char*)&metadata, BLOCKSIZE);
	*inodeptr = metadata.inodes[r];
}

void write_inode(int mount_point, int inodenum, struct inode_t *inodeptr){
	/*
		* Read the metadata in block 2 or 3
		* Decrypt the metadata block if its an encrypted system
		* Update the inode entry in the metadata block using the memory buffer
		* Encrypt the metadata block if its an encrypted system
		* Write back the metadata block to the disk
	*/

	struct metadata_t metadata;
	int d=inodenum/16, r=inodenum%16;
	readblock(mounts[mount_point].device_fd, 1+d, (char*)&metadata);
	if(mounts[mount_point].fs_number==1)
		decrypt(mounts[mount_point].key, (char*)&metadata, BLOCKSIZE);
	metadata.inodes[r] = *inodeptr;
	if(mounts[mount_point].fs_number==1)
		encrypt(mounts[mount_point].key, (char*)&metadata, BLOCKSIZE);
	writeblock(mounts[mount_point].device_fd, 1+d, (char*)&metadata);
}

int alloc_datablock(int mount_point){
	/*
		* Checks if there are any free blocks (max number of blocks are device size in superblock)
		* Update the block bitmap and used_blocks 
		
		* Return value: -1,				error
						 block number, 	success
	*/

	struct superblock_t superblock;
	read_superblock(mount_point, &superblock);

	if(superblock.used_blocks == superblock.disk_size)
		return -1;

	for(int i=0; i<MAX_BLOCKS; i++){
		if(superblock.block_bitmap[i]==0){
			superblock.block_bitmap[i]=1;
			superblock.used_blocks++;
			write_superblock(mount_point, &superblock);
			return i;
		}
	}
}

void free_datablock(int mount_point, int blocknum){
	/*
		* Updates the block bitmap and used_blocks in the superblock
	*/

	struct superblock_t superblock;
	read_superblock(mount_point, &superblock);
	superblock.block_bitmap[blocknum] = 0;
	superblock.used_blocks--;
	write_superblock(mount_point, &superblock);
}


void read_datablock(int mount_point, int blocknum, char *buf){
	/*
		* Read the block into the memory buffer
		* Decrypt the block if its an encrypted system
	*/

	readblock(mounts[mount_point].device_fd, blocknum, buf);
	if(mounts[mount_point].fs_number==1)
		decrypt(mounts[mount_point].key, buf, BLOCKSIZE);
}

void write_datablock(int mount_point, int blocknum, char *buf){
	/*
		* Encrypt the memory buffer if its an encrypted system
		* Write the metadata buffer to the block in disk
	*/

	if(mounts[mount_point].fs_number==1)
		encrypt(mounts[mount_point].key, buf, BLOCKSIZE);
	writeblock(mounts[mount_point].device_fd, blocknum, buf);
}