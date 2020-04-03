KERNDIR:=/lib/modules/5.3.0-45-generic/build
PWD:=$(shell pwd)
obj-m:=driver_0.o
all:
	make -C $(KERNDIR) M=$(PWD) modules
clean:
	make -C $(KERNDIR) M=$(PWD) clean
