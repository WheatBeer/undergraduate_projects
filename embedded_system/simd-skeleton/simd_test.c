#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

int device_ctrl = -1;

int wr_len = 0;
int rd_len = 0;

unsigned rd_value[16] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

unsigned wr_value1[12] = {10, 20, 30, 40, 1, 2, 3, 4, 0x01, 0, 0, 2};
unsigned wr_value2[12] = {10, 20, 30, 40, 1, 2, 3, 4, 0x12, 0, 0, 2};
unsigned wr_value3[12] = {10, 20, 30, 40, 1, 2, 3, 4, 0x20, 0, 0, 2};

int main(int argc, char **argv) {
	
	if(argc > 1) {
		printf("Just one argument is available\n");
		printf("Usage: %s", argv[0]);
	}
	printf("A simple program to test simd driver\n");
	
	// Step1: Open your device
	device_ctrl = open("/dev/zynq_simd", O_RDWR);
	if(device_ctrl < 0) {
		printf("device open failed\n");
		return 1;
	}
	printf("device opened with fd number of %d\n", device_ctrl);
	
	// Step2: Test your device driver's write function.
	// Step3: Test your device driver's read function.
	
	// Vector_c = Vector_a * Vector_b
	wr_len = write(device_ctrl, wr_value1, sizeof(unsigned)*12);
	rd_len = read(device_ctrl, rd_value, sizeof(unsigned)*16);
	printf("write length : %d, read length : %d\n", wr_len, rd_len);
	for(int i = 0; i < 16; i++){
		printf("%p : %d\n",rd_value+i,rd_value[i]);
	}
	
	// Vector_c = Vector_a & constant
	wr_len = write(device_ctrl, wr_value2, sizeof(unsigned)*12);
	rd_len = read(device_ctrl, rd_value, sizeof(unsigned)*16);
	printf("write length : %d, read length : %d\n", wr_len, rd_len);
	for(int i = 0; i < 16; i++){
		printf("%p : %d\n",rd_value+i,rd_value[i]);
	}
	
	// Vector_c = Vector_b + constant
	wr_len = write(device_ctrl, wr_value3, sizeof(unsigned)*12);
	rd_len = read(device_ctrl, rd_value, sizeof(unsigned)*16);
	printf("write length : %d, read length : %d\n", wr_len, rd_len);
	for(int i = 0; i < 16; i++){
		printf("%p : %d\n",rd_value+i,rd_value[i]);
	}
	// Step4: Close your device.
	close(device_ctrl);
	return 0;
}