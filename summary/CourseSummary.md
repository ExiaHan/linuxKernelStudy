#Linux课程总结

韩洋 + 原创作品转载请注明出处 + 《Linux内核分析》MOOC课程http://mooc.study.163.com/course/USTC-1000029000


###写在开始

课程结束了，但是Linux学习没有尽头，学习Linux要走的路还很长，八周的时间，我们了解了进程调度、内核启动过程、中断、系统调用过程、Fork、Execute。下面结合这八周的学习经验谈谈感想。

###对Linux的认识

#####什么是Linux内核
如课程中所述，Linux Kernel是一系列中断服务和内核线程的集合，Linux Kernel向下与硬件设备交互，把上层程序对硬件的操作请求在对应的硬件上完成，向上为应用程序和用户提供屏蔽了具体实现过程的服务，也即Unix所提倡的“提供机制而非策略”设计哲学。另外需要补充的是，大部分通常都会把发行版和Linux混为一谈，真正意义上的Linux，仅仅指Linux Kernel，或者说Linux就是一个内核，至于其上运行的一些框架和软件，如XServer，Gnome和KDE，和一些应用，对于Linux内核来说都只是运行在其上的应用而已。像Fedora，Ubuntu等说白了都只是对Linux内核和一些应用的打包，所以才会叫“发行版”。

#####学习心得
学习不能盲目，对于Linux内核这个庞然大物，如何学习呢？我们要从“Linux内核是一系列中断服务和内核线程的集合”这个总纲领出发，我们就可以向外发散：系统如何启动？加电后BIOS会把磁盘指定位置的程序装入内存运行起来，这个程序将会引导内核启动。用户程序如何运行？需要系统把它装入内存。怎么装入内存？需要系统调用。为什么需要系统调用？为了安全性和通用性考虑。系统调用怎么被触发？需要中断服务。中断了那当前程序怎么办？因为有中断上下文保存，所以可以还原。执行新城序旧程序为什么还能存在？因为是Fork的。为什么有的执行后原来的程序就没了？因为那是execute。现在有这么多程序，怎么让它们都能完成运行？别怕，我们有进程调度。这么多程序，就这么点内存，说好的每个都有4G？哈哈哈，我们可以做虚拟地址空间和页面映射还有交换。进程间如何通信？我们有管道和本地socket。外设为什么一插上就能用？因为他们的驱动模块被事先集成了呀。那我们自己需要保存文件供以后使用呢？这个文件系统可以做。什么是文件系统？系统对存储设备的管理方式。等等等等。。。明白了核心功能后，我们可以通过不断的向外发散和摸索，最终窥得全貌。这也是学习内核的思路，毕竟内核如此庞大，不可能一行行的读懂其代码，只能有目的的一段段去了解，最终把各个已经了解的功能组合起来，这样我们眼前将会出现完整的Linux内核全貌。

###本次课程所有作业：

+ 第一周：http://www.cnblogs.com/exiahan/p/4310010.html
+ 第二周：https://github.com/ExiaHan/linuxKernelStudy/blob/master/secondWeekend/processSwitch.md
+ 第三周：https://github.com/ExiaHan/linuxKernelStudy/blob/master/thirdWeekend/analyseKernelStart.md
+ 第四周：https://git.oschina.net/exiahan/LinuxKernelStudy/blob/master/4/asmSCI.md
+ 第五周：https://git.oschina.net/exiahan/LinuxKernelStudy/blob/master/5/analysis_a_sys_call.md
+ 第六周：https://git.oschina.net/exiahan/LinuxKernelStudy/blob/master/6/analysisFork.md
+ 第七周：https://git.oschina.net/exiahan/LinuxKernelStudy/blob/master/7/loadAndStartExe.md
+ 第八周：https://git.oschina.net/exiahan/LinuxKernelStudy/blob/master/8/traceSchedule.md

###总结

通过学习本门课程，对Linux的一些核心功能实现有了一定了解，但是最重要的是学到了“如何研究Linux内核”的学习方法，Linux内核如此庞大， 所以一行行的看代码，虽然可能可以学完，但是花费的时间一定是巨大的，相比之下，如果有目的，带着问题，一块块，一个功能一个功能的去学习，那么效率将会提高很多。

对于本门课程，最大的遗憾是因为其他课程对时间的挤压，导致不能完全的投入去弄懂课程中提到的相关的模块实现的一些比较重要的细节，作业中依然有很多内容待完善，以后将慢慢完善。