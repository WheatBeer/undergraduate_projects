MODULE_FILENAME = hw1_module

obj-m += $(MODULE_FILENAME).o
KO_FILE = $(MODULE_FILENAME).ko

all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean

insert:
	sudo insmod $(KO_FILE)

remove:
	sudo rmmod $(MODULE_FILENAME)
