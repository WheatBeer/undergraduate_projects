MODULE_FILENAME = hw2

obj-m += $(MODULE_FILENAME).o
KO_FILE = $(MODULE_FILENAME).ko

all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean

insert:
	sudo insmod $(KO_FILE) period=5

remove:
	sudo rmmod $(MODULE_FILENAME)
