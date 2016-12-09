ee468-DeviceDriver
==================

Instructions for installation (`sudo` may be necessary for certain
parts):

    make
    mknod /dev/ee468Driver c 60 0
    insmod ee468Driver.ko
    chmod 666 /dev/ee468Driver

Can then test the device driver by running:

	./test.out

Can view debug output with:

	dmesg

(Experimental) Test `make` commands for inserting/deleting the module:

    make unset
    make setup


