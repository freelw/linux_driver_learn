BASEINCLUDE ?= /home/wangli/project/runninglinuxkernel_4.0
PWD:=$(shell pwd)
read_write_0-objs := my_test.o
obj-m := read_write_0.o
all:
	$(MAKE) -C $(BASEINCLUDE) M=$(PWD) modules
clean:
	$(MAKE) -C $(BASEINCLUDE) SUBDIRS=$(PWD) clean
	rm -f *.ko


