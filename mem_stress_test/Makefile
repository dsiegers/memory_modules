obj-m += stress.o


all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean

test:
	sudo dmesg -C
	sudo insmod hello.ko
	sudo rmmod hello.ko
	sudo insmod hello.ko
	sudo rmmod hello.ko
	sudo insmod hello.ko
	sudo rmmod hello.ko
	sudo insmod hello.ko
	sudo rmmod hello.ko
	sudo insmod hello.ko
	sudo rmmod hello.ko
	dmesg
