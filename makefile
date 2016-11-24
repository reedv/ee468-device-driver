obj-m := driver.o

KDIR = /usr/src/linux-headers-4.4.0-47-generic

all:
	$(MAKE) -C $(KDIR) SUBDIRS=$(PWD) modules
