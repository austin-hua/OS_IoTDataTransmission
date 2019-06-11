# IoT Data Transmission
*Implementation of memory pages and serial communication mechanisms*<br>

### Program Architecture
./master_device : device module for master server<br>
./slave_device  : device module for slave client<br>
./ksocket: device module for kernel socket<br>
./data   : input/output data<br>
./user_program : user program "master" and "slave"

### Pre-Compilation
Before compiling, make sure you are running the correct version of kernel. We are using Kernel v. 4.14.25. You can check which version you are using with the command <em>uname -r</em>. There are fundamental differences in certain ksocket functions between older kernel versions and 4.14.25 that make backwards compatibility impossible. Don't learn this the hard way. Also, install the *libelf-dev*, *libncurses5-dev*, and *libssl-dev* packages to be able to run compile.sh.

(This part is for team 11 reference)
If you don't have this kernel version, run the following commands (as 4.14.25 can't be obtained from apt-get - you could also just use the procedure described in HW1).<br>
wget https://kernel.ubuntu.com/~kernel-ppa/mainline/v4.14.25/linux-headers-4.14.25-041425-generic_4.14.25-041425.201803091130_amd64.deb<br>
wget https://kernel.ubuntu.com/~kernel-ppa/mainline/v4.14.25/linux-headers-4.14.25-041425_4.14.25-041425.201803091130_all.deb<br>
wget https://kernel.ubuntu.com/~kernel-ppa/mainline/v4.14.25/linux-image-4.14.25-041425-generic_4.14.25-041425.201803091130_amd64.deb<br>
sudo dpkg -i linux-headers-4.14* linux-image-4.14* <br>

Restart, hold shift to open GRUB, and select the correct kernel.<br>

### Compilation
Next switch to super user. Use the command <em>sudo -s</em>.
Navigate to our src folder. Execute compile.sh using either of the following commands.<br>
<em>[sudo] ./compile.sh<br>
[sudo] sh compile.sh</em><br>
If even as root user permission is still denied, you must execute <em>chmod +x compile.sh</em> to give root (or whatever your super username is) access to opening this file. For this reason, we recommend simply using the last command <em>sh</em> as it does not require adding extra permissions.<br>
Afterwards, execute <em>lsmod</em>. The first three modules that appear should be slave_device, master_device, and ksocket. If this is not displayed, the modules are not being compiled properly.<br>

### Execution
After you are finish compiling, run these shell files.<br>
<em>generate.sh</em><br>
This file will create all data needed.

### Post-Execution
<em>clean.sh</em><br>
This shell file will remove all files generated by the program and will call <en>removeModules.sh</en> to remove loaded modules.<br>
<em>removeModules.sh</em><br>
This will be useful for testing in different environments. The three modules, slave_device, master_device, and ksocket, once created, are not automatically unloaded. Therefore, whenever switching testing environments, this shell file should be run using 'sh removeModules.sh' in order to test compiling in a new environment. Note: This functionality will be automatically invoked in <em>clean.sh</em>. Clean.sh has sometimes malfunctioned, however, and also will erase all other program-generated data in the folders. Therefore, we decided to separate these functions into two files.<br>

### Cool Reference Material
[Project description](http://rswiki.csie.org/dokuwiki/courses:107_2:project_2)<br>
[Char driver information](https://static.lwn.net/images/pdf/LDD3/ch03.pdf)<br>
[Switching kernel versions](https://askubuntu.com/questions/700214/how-do-i-install-an-old-kernel)<br>
[mmap information](http://man7.org/linux/man-pages/man2/mmap.2.html)<br>
[Bash reference material 1](https://stackoverflow.com/questions/9612090/how-to-loop-through-file-names-returned-by-find/9612232)<br>
[Bash reference material 2](https://www.cyberciti.biz/faq/bash-for-loop/)<br>
[Bash reference material 3](https://unix.stackexchange.com/questions/346388/can-i-find-all-files-with-the-log-extension-and-order-by-file-size)<br>
[more mmap information](https://www.poftut.com/mmap-tutorial-with-examples-in-c-and-cpp-programming-languages/)<br>
*Referenced repos*<br>
[GaryChenMesser's repo (compatible with Linux 4.14.25-generic)](https://github.com/GaryChenMesser/os_project2_2018)<br>
Acknowledgement: After running into several errors using multiple different implementations of mmap, we reviewed and based much of our current mmap implementation on the above implementation.<br>
[yccyenchicheng's repo](https://github.com/yccyenchicheng/os_project2_sp18)<br>
[b05902046's repo](https://github.com/b05902046/OS-Project-2)<br>
[nemotiger's repo (running Linux 4.15.0-23-generic)](https://github.com/nemotiger/OS-Project2)<br>
[qazwsxedcrfvtg14's repo](https://github.com/qazwsxedcrfvtg14/OS-Proj2)<br>
[MarvinChung's repo](https://github.com/MarvinChung/os_project1/tree/master/os_project2)<br>

### Group Members
[B06902101 韓哈斯](https://github.com/Gearlad) <br>
[B06902098 李恩慈](https://github.com/B06902098) <br>
[B06902086 柯燕玲](https://github.com/swallow26) <br>
[B05902099 曾彬輝](https://github.com/navenoc13)

-----------
### TODO
<p>
Add sample source code and base files [✓]<br>
Test source code [✓]<br>
Research mmap [✓]<br>
  
*Architecture*
ksocket functional [✓]<br>
master_device.c functional [✓]<br>
slave_device.c functional [✓]<br>
master.c functional [✓]<br>
slave.c functional [✓]<br>
clean.sh functional [✓]<br>
removeModules.sh functional [✓]<br>
compile.sh functional [✓]<br>
generate.sh functional [✓]
</p>
