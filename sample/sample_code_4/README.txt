This is the source code for operating systems project 2. Note that this code is optimised for Linux 4.14.25

./master_device : device module for master server
./slave_device  : device module for slave client
./ksocket: device module including the funtions used for kernel socket
./data   : input/output data
./user_program : user program "master" and "slave"


To use this code:
1. change to super user
2. execute "./compile.sh" to compile codes and install modules
3. follow the input instructions in the specification,
i.e.
./master ../data/file1_in mmap
./slave ../data/file1_out fcntl 127.0.0.1

Make sure that you have run "./user_program" when you execute user programs.
Though the execution order of user program "master" and "slave" doesn't matter,
it's suggested to execute "master" first to get more precise transmission time.
