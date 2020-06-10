KERNDIR:=/home/wangli/project/linux-4.4.16
PWD:=$(shell pwd)
obj-m:=myfs.o
all:
	make -C $(KERNDIR) M=$(PWD) modules
clean:
	make -C $(KERNDIR) M=$(PWD) clean
