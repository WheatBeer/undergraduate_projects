#ifndef __SIMD_DRIVER_H
#define __SIMD_DRIVER_H

#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/fs.h>
// #include <asm/uaccess.h>
#include <linux/uaccess.h>
#include <linux/slab.h>
#include <asm/io.h>

#define SIMD_DEVICE_MAJOR 234
#define SIMD_DEVICE_NAME "zynq_simd"
// TODO: Fill address of your device
#define SIMD_DEVICE_ADDRESS 0x43C00000
#define SIMD_DEVICE_RANGE 0x1000

int simd_open(struct inode* inode, struct file* filep);
int simd_close(struct inode* inode, struct file* filep);
ssize_t simd_write(struct file* filep, const char* user_buffer, size_t length, loff_t* f_pos);
ssize_t simd_read(struct file* filep, char* user_buffer, size_t length, loff_t* f_pos);

// TODO: Note that you can set global variables here.
void *simd_virtual_addr = NULL;

#endif