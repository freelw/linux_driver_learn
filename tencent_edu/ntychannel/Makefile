KERNDIR:=/home/wangli/project/linux-4.4.16
PWD:=$(shell pwd)
obj-m:=ntychannel.o
ntychannel-objs := 0voice_channel.o
all:
	make -C $(KERNDIR) M=$(PWD) modules
clean:
	make -C $(KERNDIR) M=$(PWD) clean
