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

#define PAGE_SIZE 4096 //4 kb
#define BUF_SIZE 512

#define slave_IOCTL_CREATESOCK 0x12345677
#define slave_IOCTL_MMAP 0x12345678
#define slave_IOCTL_EXIT 0x12345679

int main (int argc, char* argv[])
{
	char buf[BUF_SIZE];
	int i, dev_fd, file_fd;// the fd for the device and the fd for the input file
	size_t ret, file_size = 0, data_size = -1, offset = 0;
	char file_name[50];a
	char method[20];
	char ip[20];
	struct timeval start;
	struct timeval end;
	double trans_time; //calulate the time between the device is opened and it is closed
	char *kernel_address, *file_address;


	strcpy(file_name, argv[1]);
	strcpy(method, argv[2]);
	strcpy(ip, argv[3]);

	//should be O_RDWR for PROT_WRITE when mmap()
	if((dev_fd = open("/dev/slave_device", O_RDWR)) < 0) {
		perror("failed to open /dev/slave_device\n");
		return 1;
	}

	gettimeofday(&start, NULL);

	if((file_fd = open(file_name, O_RDWR | O_CREAT | O_TRUNC)) < 0) {
		perror("failed to open input file\n");
		return 1;
	}

	//0x12345677 : connect to master in the device
	if(ioctl(dev_fd, 0x12345677, ip) == -1) {
		perror("ioctl create slave socket error\n");
		return 1;
	}

    write(1, "ioctl success\n", 14);

    //char *mmapper;

	switch(method[0])
	{
		case 'f': //fcntl : read()/write()
			do {
				ret = read(dev_fd, buf, sizeof(buf)); // read from the the device
				write(file_fd, buf, ret); //write to the input file
				file_size += ret;
			} while(ret > 0);

			break;

		default: //mmap
			/*mmapper = mmap(NULL, file_size, PROT_READ, MAP_SHARED, file_fd, 0);

			if(mmapper == MAP_FAILED) {
				perror("mmap creation failed\n");
				return -1;
			}*/

			while(1) {

				ret = ioctl(dev_fd, slave_IOCTL_MMAP);

				if(ret == 0) { // success return value
					file_size = offset;
					break;
				}

				if(ret == -1) {
					perror("ioctl failed");
					return -1;
				}

				//Function parameters: int posix_fallocate(int fd, off_t offset, off_t len);
				posix_fallocate(file_fd, offset, ret); //ensure disk space allocation for file_fd
				file_address = mmap(NULL, ret, PROT_WRITE, MAP_SHARED, file_fd, offset);
				kernel_address = mmap(NULL, ret, PROT_READ, MAP_SHARED, dev_fd, offset);

				memcpy(file_address, kernel_address, ret);
				offset += ret;
			}

			//int ioctl(int fd, unsigned long request, ...);
			unsigned long request = 7122;
			ioctl(dev_fd, request);

			/*if(munmap(mmapper, file_size) == -1) {
				perror("unmapping failed\n");
				return -1;
			}*/
	}


	// end receiving data, close the connection
	if(ioctl(dev_fd, slave_IOCTL_EXIT) == -1) {
		perror("ioclt client exits error\n");
		return 1;
	}
	gettimeofday(&end, NULL);
	trans_time = (end.tv_sec - start.tv_sec)*1000 + (end.tv_usec - start.tv_usec)*0.0001;
	printf("Transmission time: %lf ms, File size: %d bytes\n", trans_time, file_size / 8);

	close(file_fd);
	close(dev_fd);
	return 0;
}


