ee468-DeviceDriver

Instructions for installation:
1. use command $ make
2. $ mknod /dev/ee468Driver c 60 0
3. $ insmod ee468Driver.ko
4. $ chmod 666 /dev/memory

Can test the device by compiling the test.c file
	$ gcc -Wall -o test.o test.c
	$ ./test.o