#Stack-Switch 对进程切换的一些理解

韩洋 + 原创作品转载请注明出处 + 《Linux内核分析》MOOC课程 http://mooc.study.163.com/course/USTC-1000029000

*本文为Mooc网相关课程作业，文中可能引用到一些网络资料，在这里对这些资料的原作者提供感谢。*

*****

##前言
现代操作系统以线程为基本调度单位，分为进程内线程调度和进程间线程调度，同时，一个线程对应一个运行栈，所以**Linux**进程调度过程中对栈的操作十分频繁，接下来将通过本次实验详细解释进程调度过程中运行栈的变化情况。

##实验准备
**写上这段是因为按照说明进行的话并不能看到教学视频上的效果，只能看到时间中断处理程序在执行**

- - -

按照作业页面的说明去**Github**上的[**mykernel**](https://github.com/mengning/mykernel) 项目按照其说明下载“linux 3.9.4内核源码”，然后clone项目到本地，解压内核，然后对内核打patch

	$patch p1 < [your patt]/mykernel/mykernel/mykernel_for_linux3.9.4sc.patch
    
*这里打完补丁后不要直接按照说明直接make，查看patch后内核文件变化可以发现patch并没有写入教学视频中的完整mykernel代码，所以我们要先把代码完整复制过去*

	$cp -rf [your path]/mykernel [your path]/linux-3.9.4/

同时为了缩减时间和使实验效果比较明显，这里我们要对mykernel/myinterrupt.c文件做一点修改，和教学视频里的一样，缩短时间中断处理函数里的时间片，如下：

	void my_timer_handler(void)
    {
  	#if 1
    	if(time_count%1000 == 0 && my_need_sched != 1)
    	{
        	printk(KERN_NOTICE ">>>my_timer_handler here<<<\n");
        	my_need_sched = 1;
    	} 
    	time_count ++ ;  
	#endif
    return;  	
	}

然后再执行编译命令

	$make allnoconfig
    $make

等待编译完成即可通过qemu虚拟机运行我们刚刚编译好的镜像来查看效果

	$qemu-system-i386 [your path]/arch/x86/boot/bzImage

#####运行效果如下：
**这里有四个进程，0, 1, 2, 3互相进行切换**
![process 0 to 1](./pic/1.png)
![process 1 to 2](./pic/2.png)
![process 2 to 3](./pic/3.png)
![process 3 to 0](./pic/4.png)
