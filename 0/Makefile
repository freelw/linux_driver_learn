KERNDIR:=/usr/src/linux-source-5.3.0/linux-source-5.3.0/
PWD:=$(shell pwd)
obj-m:=driver_0.o
all:
	make -C $(KERNDIR) M=$(PWD) modules
clean:
	make -C $(KERNDIR) M=$(PWD) clean
