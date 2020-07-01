BASEINCLUDE ?= /home/wangli/project/runninglinuxkernel_4.0
PWD:=$(shell pwd)
misctest-objs := my_test.o
obj-m := misctest.o
all:
	$(MAKE) -C $(BASEINCLUDE) M=$(PWD) modules
clean:
	$(MAKE) -C $(BASEINCLUDE) SUBDIRS=$(PWD) clean
	rm -f *.ko


