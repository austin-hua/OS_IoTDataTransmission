# IoT Data Transmission
### Operating Systems Project 2
Implementation of memory pages and serial communication mechanisms

### Compilation
Before compiling, make sure you are running the correct version of kernel. We are using Kernel v. 4.14.25. You can check which version you are using with the command <em>uname -a</em>. There are fundamental differences in certain ksocket functions between older kernel versions and 4.14.25 that make backwards compatibility impossible. Don't learn this the hard way.

Next switch to super user. Use the command <em>sudo -s</em>.
Navigate to our src folder. Execute compile.sh using any of the following commands.
<p><em>./compile.sh<br>
    sudo ./compile.sh<br>
    sh compile.sh</em>
</p>
If even as root user permission is still denied, you must execute <em>chmod +x compile.sh</em> to give root (or whatever your super username is) access to opening this file. For this reason, we recommend simply using the last command <em>sh</em> as it does not require adding extra permissions.

### Cool Reference Material
<p>
Project description: http://rswiki.csie.org/dokuwiki/courses:107_2:project_2<br>
Char driver information: https://static.lwn.net/images/pdf/LDD3/ch03.pdf<br>
https://askubuntu.com/questions/700214/how-do-i-install-an-old-kernel<br>
Referenced Repo: https://github.com/yccyenchicheng/os_project2_sp18<br>
Referenced Repo: https://github.com/b05902046/OS-Project-2<br>
Referenced Repo: https://github.com/GaryChenMesser/os_project2_2018<br>
Referenced Repo (linux 4.15.0-23-generic): https://github.com/nemotiger/OS-Project2
</p>

### Group Members
<p>[B06902101 韓哈斯](https://github.com/Gearlad)<br>
[B06902098 李恩慈](https://github.com/B06902098)<br>
[B06902086 柯燕玲](https://github.com/swallow26)<br>
[B05902099 曾彬輝](https://github.com/navenoc13)
</p>

### TODO
<p>
  Add sample source code and base files [✓]<br>
  Test source code [ ]<br>
  Research mmap [ ]<br>
  Functional prototype [ ]
</p>
