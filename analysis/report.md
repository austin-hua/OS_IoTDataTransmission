# Project Report

## Design
The entire framework runs on Linux's mmap (memory map) library. mmap is pretty much a function for allocating memory on the heap. It's called by C's malloc() function, for example. Cool stuff.<br>
So the basic idea is, we're comparing the speed of mmap with normal file I/O.<br>

Master & Slave Description <br>
#### Master
This kernel program is designed to accept user program received and send it to the master device. The master program will be loaded after the master_module is loaded.

#### Slave
This kernel program is designed to receive data from the master device and transmit it to the user program. This program will only be executed after both slave_module and master_module are loaded. The slave program is responsible for setting the master IP address to connect to. The slave device will receive data from the master device, which in turn will return this data to the slave program.

### Execution Time Results
#### mmap
#### File I/O

## Analysis

### Contributions
B06902101 韓哈斯: Designed program architecture, researched mmap, wrote report.md<br>
B06902098 李恩慈: <br>
B06902086 柯燕玲: <br>
B05902099 曾彬輝: 
