#include "simd_driver.h"

// TODO: Note that you can set global variables here.
void *device_buffer = NULL;

static const struct file_operations simd_fops =
{
	// TODO: Fill this struct to match system call functions (open, close, etc.)
	// and functions from your device driver code.
	.open    = simd_open,
	.release = simd_close,
	.write   = simd_write,
	.read    = simd_read,
};

static int __init simd_init(void){
	printk(KERN_INFO "Initiating simd_driver\n");

	// Step1: Register your device driver to the OS
	if(register_chrdev(SIMD_DEVICE_MAJOR, SIMD_DEVICE_NAME, &simd_fops) < 0)
		printk(KERN_ALERT"<SIMD_DRIVER> Cannot open %s module**********************\n", SIMD_DEVICE_NAME);

	simd_virtual_addr = ioremap(SIMD_DEVICE_ADDRESS, SIMD_DEVICE_RANGE);
	printk(KERN_DEBUG"<SIMD_DRIVER> simd_virtual_addr = 0x%lx\n", (unsigned int*)simd_virtual_addr);
	printk(KERN_DEBUG"<SIMD_DRIVER> Controller init success. Major number: %d\n", SIMD_DEVICE_MAJOR);
	// Step2: Allocate buffer in kernel memory space
	device_buffer = (void *)kmalloc(1024, GFP_KERNEL);

	return 0;
}

static void __exit simd_exit(void){
	printk(KERN_INFO "Exiting simd_driver\n");

	// Step1: Unregister your device driver from the OS
	iounmap(simd_virtual_addr);
	unregister_chrdev(SIMD_DEVICE_MAJOR, SIMD_DEVICE_NAME);
	// Step2: Free buffer allocated from init function from kernel memory space
	// to prevent memory leaks.
	kfree(device_buffer);
	return;
}

int simd_open(struct inode* inode, struct file* filep) {
	printk(KERN_INFO"<SIMD_DRIVER> simd_zynq is opened. \n");

	// Step1: Print student ID and name of your teammates using
	// printk function using KERN_ALERT instead of KERN_INFO.
	printk(KERN_ALERT"<SIMD_DRIVER> simd_open() Group: 0 Student ID: xxxxxxxx Name: Sungmin Ryu\n");
	return 0;
}

int simd_close(struct inode* inode, struct file* filep) {
	printk(KERN_INFO"<SIMD_DRIVER> simd_zynq is closed\n");

	// Step1: Print student ID and name of your teammates using
	// printk function using KERN_ALERT instead of KERN_INFO.
	printk(KERN_ALERT"<SIMD_DRIVER> simd_close() Group: 0 Student ID: xxxxxxxx Name: Sungmin Ryu\n");
	return 0;
}

ssize_t simd_write(struct file* filep, const char* user_buffer, size_t length, loff_t* f_pos) {
	int err;
	int i;
	printk(KERN_INFO"<SIMD_DRIVER> called simd_write()\n");

	// Step1: Check the value of length to determine erroneous write requests.
	if(length != sizeof(unsigned) * 12) { 
		printk(KERN_ALERT"<SIMD_DRIVER> write length should be 48byte\n");
		printk(KERN_DEBUG"<SIMD_DRIVER> current write length %d\n", length);
		return length;
	}

	// Step2: If the value of length is okay, write data in the user space to the device.
	err = copy_from_user(device_buffer, user_buffer, length);
	if(err < 0) {
		printk(KERN_ALERT"<SIMD_DRIVER> device write error; error code %d\n", err);
		return err;
	}
	for(i = 0; i < 12 ; i++){
		*(volatile unsigned *)(simd_virtual_addr+i) = *(volatile unsigned *)(device_buffer+i);
	} 

	return length;
}

ssize_t simd_read(struct file* filep, char* user_buffer, size_t length, loff_t* f_pos) {
	int err;
	int i;
	printk(KERN_INFO"<SIMD_DRIVER> called simd_read()\n");

	// Step1: Check the value of length to determine erroneous read requests.
	if(length != sizeof(unsigned) * 16) {
		printk(KERN_ALERT"<SIMD_DRIVER> read length should be sizeof int or long long\n");
		printk(KERN_DEBUG"<SIMD_DRIVER> current read length %d\n", length);
		return length;
	}

	// Step2: If the value of length is okay, read data from device it and send it to user space.
	for(i = 0; i < 16 ; i++){
		*(volatile unsigned *)(device_buffer+i) = *(volatile unsigned *)(simd_virtual_addr+i);
	}
	err = copy_to_user(user_buffer, device_buffer, length);
	if(err < 0) {
		printk(KERN_ALERT"<SIMD_DRIVER> device read error; error code %d\n", err);
		return err;
	}

	return length;
}

module_init(simd_init);
module_exit(simd_exit);
