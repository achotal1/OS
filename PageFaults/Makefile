
ifeq ($(KERNELRELEASE),)
	KERNELDIR ?= /lib/modules/$(shell uname -r)/build
	PWD := $(shell pwd)

modules:
	make -C $(KERNELDIR) M=$(PWD) modules
	gcc user_space.c -o user_space
modules_install:
	sudo make -C $(KERNELDIR) M=$(PWD) modules_install
r:
	sudo rmmod kprobe.ko
i:
	sudo insmod kprobe.ko
user:
	sudo ./user_space
sys:
	sysbench --test=cpu --cpu-max-prime=10000 run	
clean:
	rm -rf *.o *~ core .depend .*.cmd *.ko *.mod.c *.mod modules.order Module.symvers .tmp_versions user_space
else
	obj-m := kprobe.o
endif



