#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <errno.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/time.h>

#define PAGE_SIZE 4096
#define BUF_SIZE 512

size_t get_filesize(const char* filename) //input file size
{
    struct stat st;
    stat(filename, &st);
    return st.st_size;
}

int main (int argc, char* argv[])
{
	char buf[BUF_SIZE];
	int i, dev_fd, file_fd;// the fd for the device and the fd for the input file
	size_t ret, file_size, tmp, offset = 0;
	char file_name[50], method[20];
	char *kernel_address = NULL, *file_address = NULL;
	struct timeval start;
	struct timeval end;
	double trans_time; //calulate time interval of device being opened to device being closed

	strcpy(file_name, argv[1]);
	strcpy(method, argv[2]);


	if((dev_fd = open("/dev/master_device", O_RDWR)) < 0) {
		perror("Error opening /dev/master_device\n");
		return 1;
	}

	gettimeofday(&start ,NULL);

	if((file_fd = open(file_name, O_RDWR)) < 0) {
		perror("Error opening input file\n");
		return 1;
	}

	if((file_size = get_filesize(file_name)) < 0) {
		perror("failed to get filesize\n");
		return 1;
	}

	//0x12345677 : create socket and accept the connection from the slave
	if(ioctl(dev_fd, 0x12345677) == -1) {
		perror("ioclt server socket creation error\n");
		return 1;
	}

	char *mmapper;

	switch(method[0]) {
		case 'f': //file I/O
			do {
				ret = read(file_fd, buf, sizeof(buf)); // read from the input file
				write(dev_fd, buf, ret); //write to the the device
			} while(ret > 0);
			break;

		default: //mmap
			//void *mmap(void *addr, size_t length, int prot, int flags, int fd, off_t offset);
			//can use map_shared or map_private
			mmapper = mmap(NULL, file_size, PROT_READ, MAP_SHARED, file_fd, 0);
			/*
			On success, mmap() returns a pointer to the mapped area.  On error,
	       	the value MAP_FAILED (that is, (void *) -1) is returned, and errno is
	       	set to indicate the cause of the error.
	       	*/
			if(mmapper == MAP_FAILED) {
				perror("mmap creation failed\n");
				return -1;
			}

			int data_size = file_size;
			//ssize_t write(int fd, const void *buf, size_t count);
			do {
				int write_location = mmapper + file_size - ret;
				if(data_size >= BUF_SIZE) {
					write(dev_fd, write_location, BUF_SIZE);
				} else {
					write(dev_fd, write_location, ret);
				}
				ret -= BUF_SIZE;
			} while(ret > 0); //end of file

			//int munmap(void *addr, size_t length);
			if(munmap(mmapper, file_size) == -1) {
				perror("unmapping failed\n");
				return -1;
			}
			break;
	}

	// end sending data, close the connection
	if(ioctl(dev_fd, 0x12345679) == -1) {
		perror("ioclt server exit error\n");
		return 1;
	}

	gettimeofday(&end, NULL);
	trans_time = (end.tv_sec - start.tv_sec)*1000 + (end.tv_usec - start.tv_usec)*0.0001;
	printf("Transmission time: %lf ms, File size: %d bytes\n", trans_time, file_size / 8);

	close(file_fd);
	close(dev_fd);

	return 0;
}
