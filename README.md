ee468-DeviceDriver
==================

Instructions for installation (`sudo` may be necessary for certain
parts; creation of `/dev/memory` might also be necessary):

    make
    mknod /dev/ee468Driver c 60 0
    insmod ee468Driver.ko
    chmod 666 /dev/memory

Can then test the device driver by running:

	./test.out

Can view debug output with:

	dmesg
