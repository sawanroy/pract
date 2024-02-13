obj-m += pcd.o
KVM = $(shell uname -r)
#KVM2 = "6.5.0-14-generic"

all:
	make -C /lib/modules/$(KVM)/build M=$(shell pwd) modules
