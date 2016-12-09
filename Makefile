obj-m += ee468Driver.o

all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules
	gcc -Wall -o test.out test.c

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
	rm test.out

setup:
	sudo mknod /dev/ee468Driver c 60 0
	sudo insmod ee468Driver.ko
	sudo chmod 666 /dev/ee468Driver

unset:
	sudo rmmod ee468Driver
	sudo rm /dev/ee468Driver
