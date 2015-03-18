#Linux内核学习第三周--从start_kernel到rest_init

**韩洋 + 原创作品转载请注明出处 + 《Linux内核分析》MOOC课程http://mooc.study.163.com/course/USTC-1000029000**

##前言
**文章里的内容部分来自网络，在这里对这些内容的原作者表示感谢**

*本文是网易Mooc课程《Linux内核分析》的第三周作业文章，欢迎阅读，如有疏漏，敬请指出，不胜感激*

##环境搭建
本文内容不依靠实验楼环境，在本机上自主搭建，本次环境搭建按照课程资料可以成功搭建，搭建教程见课程第三周课件《构造一个简单的linux系统》，唯一需要注意的一点是make menuconfig中相关调整：
- 如果机器是64位机器，则需要取消选中 “64-bit kernel”
- 再依次选择kernel hacking-->Compile-time checks and compiler options-->Compile the kernel with debug info，使我们即将编译的内核会带上调试信息

**上述过程完成后即可开始实验**

在完成上述配置后，执行如下命令启动qemu和gdb，开始调试[注意，有些发行版如Archlinux，需要执行的命令如下]
```
	$qemu-system-i386 -kernel [your path]/linux-3.18.6/arch/x86/boot/bzImage -initrd [your path]/rootfs.img -s -S
    $gdb -q
	$(gdb)file [your path]/linux-3.18.6/vmlinux
    $(gdb)target remote :1234
    $(gdb)b start_kernel
  	$(gdb)layout split
 	$(gdb)c
```
这里的缺别是Archlinux上对qemu做了细分，启动指定架构的内核要用对应的qemu命令，这里就是qemu-system-i386

执行完上述命令后，我们会看到qemu停留在start_kernel调用前，截图如下：

![b start_kernel](./pic/001.jpg)