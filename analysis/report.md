# Project Report

## Design
The entire framework runs on Linux's mmap (memory map) library. mmap is pretty much a function for allocating memory on the heap. It's called by C's malloc() function, for example. Cool stuff. It is an essential system call for a variety of programming applications. <br>

In this experiment, we converted the files provided in the sample code (file_in) to .in files. Our program is designed to read all '.in' files located in the ./data library. The corresponding '.out' file is generated upon data transmission.

### Master & Slave Description
Master/Slave is a model of communication where the Master process has control over the Slave process(es). 
#### Master
This kernel program is designed to accept user program received and send it to the master device. The master program will be loaded after the master_module is loaded.

#### Slave
This kernel program is designed to receive data from the master device and transmit it to the user program. This program will only be executed after both slave_device and master_device are loaded. The slave program is responsible for setting the master IP address to connect to. The slave device will receive data from the master device, which in turn will return this data to the slave program.

## Analysis
Time is measured in ms.<br>
f and m are file I/O and mmap, as they are referred to in the source code.<br>
File size is measured in bytes. As specified in the source code, PAGE_SIZE is 4096 bytes or 4 kb.

### Dataset 1
|File size (in bytes, sample input, .in extension)|Master (method f)|Slave (method f)|Master (method m)|Slave (method m)|
|-------------------------------------------------|-----------------|----------------|-----------------|----------------|
| 4                                               | 0.618           | 0.32           | 0.0742           | 0.0519         | 
| 9695                                            | 1.3312          | 0.6381         | 0.4087           | 0.1012         | 
| 1502860                                         | 13.7193         | 13.7708        | 5.0837           | 4.7855         | 
| 577                                             | 18.9267         | 18.76          | 0.1427           | 0.0399         | 

### Dataset 2
|File size (in bytes, sample input, .in extension)|Master (method f)|Slave (method f)|Master (method m)|Slave (method m)|
|-------------------------------------------------|-----------------|----------------|-----------------|----------------|
|4                                                |0.3081           |0.22            |0.1369           |0.0547          |
|9695                                             |0.08             |0.2076          |0.1649           |0.1853          |
|1502860                                          |4.3947           |5.6345          |2.7322           |8.815           |
|577                                              |0.3103           |0.2322          |0.8319           |0.0502          |


### Dataset 3
|File size (in bytes, sample input, .in extension)|Master (method f)|Slave (method f)|Master (method m)|Slave (method m)|
|-------------------------------------------------|-----------------|----------------|-----------------|----------------|
|4                                                |0.0271           |0.0323          |0.022            |0.5452          |
|9695                                             |0.5594           |0.3188          |0.5093           |0.3756          |
|1502860                                          |6.1279           |13.0579         |6.7043           |9.7628          |
|577                                              |0.0234           |0.21            |0.2434           |0.0715          |


### Dataset 4
|File size (in bytes, sample input, .in extension)|Master (method f)|Slave (method f)|Master (method m)|Slave (method m)|
|-------------------------------------------------|-----------------|----------------|-----------------|----------------|
|4                                                |0.0618           |0.0605          |0.0317           |0.0468          |
|9695                                             |0.0843           |0.698           |0.6532           |0.2291          |
|1502860                                          |8.2803           |13.3495         |7.7723           |12.6573         |
|577                                              |0.342            |0.1962          |2.1817           |0.7895          |


### Dataset 5
|File size (in bytes, sample input, .in extension)|Master (method f)|Slave (method f)|Master (method m)|Slave (method m)|
|-------------------------------------------------|-----------------|----------------|-----------------|----------------|
|4                                                |0.7712           |0.2513          |0.4338           |1.6079          |
|9695                                             |0.2554           |0.47            |0.3716           |1.6836          |
|1502860                                          |8.3034           |10.2201         |5.9214           |11.5755         |
|577                                              |0.1475           |0.308           |0.8359           |0.3627          |

## Weaknesses in This Analysis
We have tried finding a consistently reliable method for unloading modules, including the provided sample code version as well as several other methods. Mutual module dependency for ksocket and master_device have made it difficult to have 100% success, however, even with the risky *rmmod -f* command. If these modules can't be unloaded, rebooting Ubuntu is the fallback.

This program is also being tested only on one computer, and the recorded times for this analysis are resultingly skewed. Results may be completely different on different devices, with variables such as LAN and internet speeds, different versions of Ubuntu kernels, a more advanced computer processor, etc. If the objective is to find accurate results in the Internet of Things regarding file I/O and socket speed differences, other IOT devices such as phones, smart watches and embedded systems may be tested as well.

### Contributions
B06902101 韓哈斯: Designed program architecture, researched mmap, wrote report.md<br>
B06902098 李恩慈: Wrote report<br>
B06902086 柯燕玲: Wrote report<br>
B05902099 曾彬輝: 
