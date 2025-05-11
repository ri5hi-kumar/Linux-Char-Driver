# Linux Char Driver
Implemented a circular queue as a linux kernel driver with blocking behaviour,
meaning the reader process will wait till there is data in the queue and then
pop the data as soon as available.

## To load and run the driver
```
$ make
$ sudo insmod final.ko

// check the driver major number in dmesg logs
$ sudo dmesg | tail
$ sudo mknod /dev/rishi c <major> <minor>

// compile userspace programs
$ gcc configurator.c -o configurator
$ ./configurator
```
