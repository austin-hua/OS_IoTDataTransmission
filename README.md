# IoT Data Transmission
### Operating Systems Project 2
Implementation of memory pages and serial communication mechanisms

### Compilation
Before compiling, make sure you are running the correct version of kernel. We are using Kernel v. 4.14.25. You can check which version you are using with the command <em>uname -r</em>. There are fundamental differences in certain ksocket functions between older kernel versions and 4.14.25 that make backwards compatibility impossible. Don't learn this the hard way. Also, install the *libelf-dev*, *libncurses5-dev*, and *libssl-dev* packages to be able to run compile.sh.

(This part is for team 11 reference)
If you don't have this kernel version, run the following commands (as 4.14.25 can't be obtained from apt-get - you could also just use the procedure described in HW1).
wget https://kernel.ubuntu.com/~kernel-ppa/mainline/v4.14.25/linux-headers-4.14.25-041425-generic_4.14.25-041425.201803091130_amd64.deb<br>
wget https://kernel.ubuntu.com/~kernel-ppa/mainline/v4.14.25/linux-headers-4.14.25-041425_4.14.25-041425.201803091130_all.deb<br>
wget https://kernel.ubuntu.com/~kernel-ppa/mainline/v4.14.25/linux-image-4.14.25-041425-generic_4.14.25-041425.201803091130_amd64.deb<br>
sudo dpkg -i linux-headers-4.14* linux-image-4.14* <br>

Restart, hold shift to open GRUB, and select the correct kernel.<br>

Next switch to super user. Use the command <em>sudo -s</em>.
Navigate to our src folder. Execute compile.sh using any of the following commands.
<p><em>./compile.sh<br>
    sudo ./compile.sh<br>
    sh compile.sh</em>
</p>
If even as root user permission is still denied, you must execute <em>chmod +x compile.sh</em> to give root (or whatever your super username is) access to opening this file. For this reason, we recommend simply using the last command <em>sh</em> as it does not require adding extra permissions.<br>
Afterwards, execute <em>lsmod</em>. The first three modules that appear should be slave_device, master_device, and ksocket. If this is not being displayed, the modules are not being compiled properly.<br>



### Cool Reference Material
[Project description](http://rswiki.csie.org/dokuwiki/courses:107_2:project_2)<br>
[Char driver information](https://static.lwn.net/images/pdf/LDD3/ch03.pdf)<br>
[Switching kernel versions](https://askubuntu.com/questions/700214/how-do-i-install-an-old-kernel)<br>
[mmap information](http://man7.org/linux/man-pages/man2/mmap.2.html)<br>
[more mmap information](https://www.poftut.com/mmap-tutorial-with-examples-in-c-and-cpp-programming-languages/)<br>
[yccyenchicheng's repo](https://github.com/yccyenchicheng/os_project2_sp18)<br>
[b05902046's repo](https://github.com/b05902046/OS-Project-2)<br>
[GaryChenMesser's repo](https://github.com/GaryChenMesser/os_project2_2018)<br>
[nemotiger's repo (running linux 4.15.0-23-generic)](https://github.com/nemotiger/OS-Project2)

### Group Members
[B06902101 韓哈斯](https://github.com/Gearlad) <br>
[B06902098 李恩慈](https://github.com/B06902098) <br>
[B06902086 柯燕玲](https://github.com/swallow26) <br>
[B05902099 曾彬輝](https://github.com/navenoc13)

### TODO
<p>
  Add sample source code and base files [✓]<br>
  Test source code [ ]<br>
  Research mmap [ ]<br>
  Functional prototype [ ]
</p>
