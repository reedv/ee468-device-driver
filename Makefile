obj-m += ee468Driver.o

all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules
	gcc -Wall -o test.out test.c

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
	rm test.out
