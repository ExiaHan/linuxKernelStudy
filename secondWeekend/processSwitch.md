#Stack-Switch 对进程切换的一些理解

韩洋 + 原创作品转载请注明出处 + 《Linux内核分析》MOOC课程 http://mooc.study.163.com/course/USTC-1000029000

*本文为Mooc网相关课程作业，文中可能引用到一些网络资料，在这里对这些资料的原作者提供感谢。*

*****

##前言
进程调度中，最为重要的就是对上下文环境的保存和运行栈的切换，甚至我们可以把进程看成就是一个运行栈[** 现代操作系统通常以线程为基本调度单位，根据具体情况又可以分为内核级的轻量级进程和用户级线程，其调度方式有一定区别，但是其中都要对运行栈进行切换操作**]，接下来将通过本次实验详细解释进程调度过程中运行栈的变化情况。

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

##代码分析
**下面我们假设一共有0, 1, 两个进程，在此基础上对进程调度中运行栈的变化进行说明**

*****


看完实验演示，下面将结合代码对运行栈的变化进行分析：
我们可以看到mykernel路径下有三个C源文件，分别是
- mypcb.h
- mymain.c
- myinterrupt.c

**其中mypcb.h里定义了**
- PCB结构
- 线程结构

**myinterrupt.c里定义了**
- 我们自己的时钟中断相应函数my_timer_handler(void)
- 调度函数 my_schedule(void)

**mymain.c作为入口，提供了如下功能**
- 入口函数my_start_kernel(void)
- 初始化0号和后继进程
- 示例进程函数

**下面从mymain.c开始分析**

前面已经说过线程切换中最为重要的是运行栈的切换和EIP的正确跳转
在**my_start_kernel**函数中
```
......
	int pid = 0;
    int i;
    /* Initialize process 0*/
    task[pid].pid = pid;
    task[pid].state = 0;/* -1 unrunnable, 0 runnable, >0 stopped */
    task[pid].task_entry = task[pid].thread.ip = (unsigned long)my_process;
    task[pid].thread.sp = (unsigned long)&task[pid].stack[KERNEL_STACK_SIZE-1];
    task[pid].next = &task[pid];
    /*fork more process */
    for(i=1;i<MAX_TASK_NUM;i++)
    {
        memcpy(&task[i],&task[0],sizeof(tPCB));
        task[i].pid = i;
        task[i].state = -1;
        task[i].thread.sp = (unsigned long)&task[i].stack[KERNEL_STACK_SIZE-1];
        task[i].next = task[i-1].next;
        task[i-1].next = &task[i];
    }
......
```
上面一段代码完成了对0号进程的初始化，同时用类似方法构造了MAX_TASK_NUM - 1个PCB以备使用。这里比较重要的就是PCB结构中的threap.sp，前面说过一个线程对应一个栈，所以在这里我们把thread.sp指向对应PCB内的char stack[KERNEL_STACK_SIZE - 1]的地方，即用这个字符数组作为运行栈，为何指向stack[KERNEL_STACK_SIZE - 1]，是因为**栈是由高地址向低地址增长**，这个概念一定要深深的印到脑海里。
上述代码做完准备工作后，下面的代码将完成对**第0号进程的启动**
```
	pid = 0;
    my_current_task = &task[pid];
	asm volatile(
    	"movl %1,%%esp\n\t" 	/* set task[pid].thread.sp to esp */
    	"pushl %1\n\t" 	        /* push ebp */
    	"pushl %0\n\t" 	        /* push task[pid].thread.ip */
    	"ret\n\t" 	            /* pop task[pid].thread.ip to eip */
    	"popl %%ebp\n\t"
    	: 
    	: "c" (task[pid].thread.ip),"d" (task[pid].thread.sp)	/* input c or d mean %ecx/%edx*/
	);
```
这里使用了内联汇编来完成对0号进程的启动，下面对这几句内联汇编做详细分析，启动过程中运行栈的变化情况如下：

*****


+ 当前运行栈情况如图：

![At Start](./pic/stack_status/pid0_0.png)

这里的ret和old ebp 是my_start_kernel的Prolog操作，ret是my_start_kernel的返回地址，old ebp是调用my_start条微博之前不是删除了吗？_kernel函数前的栈基址。之所以在old ebp之后留有空隙是因为esp到ebp之间还要有局部变量，也可能会有内存对齐空出的空间，所以当前栈顶距[esp]离当前栈基址[ebp]会有一定距离。

**这里里需要注意的是，如果内核不是通过call指令调用的my_start_kernel的话，可能内存中没有ret这条，因为即将启动的0号进程不会退出，所以就算没有ret也没有影响**


	"movl %1,%%esp\n\t" 	/* set task[pid].thread.sp to esp */

本条指令把0号进程当前的栈顶地址放入esp，，此条指令执行后，我们可以认为栈发生了半切换，之所以没有说栈完全切换，因为这里并没有同时更改ebp的值。同时，因为此时0号进程并未运行，所以栈空，esp指向stack[KERNEL_STACK_SIZE - 1]，如图：

![1 inc](./pic/stack_status/pid0_1.png)

这里需要注意的是，前面初始化threa.sp时是

	task[i].thread.sp = (unsigned long)&task[i].stack[KERNEL_STACK_SIZE-1];

所以esp指向的是stack[KERNEL_STACK_SIZE - 1]，并不是有些人认为的esp应该指向stack数组后面紧接的第一个内存地址。

	pushl task[pid].thread.sp

本条指令把当前esp的值再次push入新栈，这里的这个操作有问题，因为并没有保存定位my_start_kernel函数运行栈的ebp值，但是因为mykernel把第0号进程作为类似于系统的** 空闲处理进程[idle]**进程，此进程是个无限循环，并不会返回，所以不会触发函数返回问题。

*****
###此处的一点思考

如果假设0号进程会返回，即我们假设OS是单任务系统，每次运行完一个进程后就回到一个输入界面等待用户输入下一个任务，则可以按照如下思路修改：
```
asm volatile(
	 "pushl %%eax\n\t" /*save eax because we will change it*/
     "movl %%esp, %%eax\n\t" /* save current esp*/
     "movl %1,%%esp\n\t" /* set task[pid].thread.sp to esp */
     "pushl %%eax\n\t" /*store the old esp in the new stack*/
     "pushl %%ebp\n\t" /* store the old ebp in the new stack*/
     "call %0" /* start process*/
     "popl %%ebp\n\t" /* when return first restore ebp*/
     "popl %%esp\n\t" /* then restore the esp*/
     "popl %%eax\n\t" /*at last restore the eax*/
     :
     : "c" (task[pid].thread.ip),"d" (task[pid].thread.sp)	/* input c or d mean %ecx/%edx*/
);
```
**大概思路就是要能保证当前的栈环境能完整恢复，这里比较绕的是我们需要先把当前esp保存到eax里，以为内下一步会切换栈顶，破坏esp内的内容，所以要提前保存，在切换到新栈顶后，我们要先把eax，即old esp入栈，然后再把old ebp入栈，如果反过来入栈，则出栈时会因为先修改了esp导致栈顶先切换而导致无法弹出真正的old ebp。**经测试修改过的代码可以正确运行。

*****


**下面继续接着mykernel提供的代码分析**

	"pushl %1\n\t" 	        /* push ebp */

按照教程里说的，这里把当前0号进程栈顶作为新的esp入栈，但是其实从上面的分析可以看出来这里是有问题的，所以这里不再解释这句的作用

   	"pushl %0\n\t" 	        /* push task[pid].thread.ip */

把0号进程的入口函数，即my_process()方法的入口地址入栈，此时栈内情况如下图：
![before ret pid 0](./pic/stack_status/pid0_2.png)

 	"ret\n\t" 	            /* pop task[pid].thread.ip to eip */

执行ret，从栈中弹出刚刚放入的my_process()入口地址，开始运行0号进程。

接下来0号进程将开始运行，同时我们提供的时钟中断处理函数也已经在运行中，按照mykernel提供的代码，当my_process()方法内的局部变量i的值为10000000时，将会检查是否需要调度，这里我们假设此时我们时钟中断函数已经设置了调度标志，则将调用sand_priority()方法给当前所有进程[在我们这里是0,1两个进程]重新分配优先级，然后调用my_schedule()方法进行进程调度。

这里为了能够看到栈的切换，我们假设进入调度函数后，会进行进程0到进程1的切换。
由于当前1号进程的pid[1].state是 "-1"，所以将会执行else分支的内容，下面对else分支内的内容进行分析：
首先，代码如下：
```
......

    else
    {
        next->state = 0;
        my_current_task = next;
    	printk(KERN_NOTICE "switch from %d process to %d process\n \
        >>>process %d running!!!<<<\n\n\n",prev->pid,next->pid,next->pid);

     /* switch to new process */
     asm volatile(	
         "pushl %%ebp\n\t" /* save ebp */
         "movl %%esp,%0\n\t" /* save esp */
         "movl %2,%%esp\n\t" /* restore esp */
         "movl %2,%%ebp\n\t" /* restore ebp */
         "movl $1f,%1\n\t" /* save eip */	
         "pushl %3\n\t"
         "ret\n\t" /* restore eip */
         : "=m" (prev->thread.sp),"=m" (prev->thread.ip)
         : "m" (next->thread.sp),"m" (next->thread.ip)
     );
    }

......
```
进入else分支后首先是把1号进程的state改为0,即runnable[可运行]，然后修改当前运行进程指针令其指向1号进程的PCB结构体，执行完后开始执行内联汇编语句，进行进程切换操作
此时栈情况如图：

![pid 0 my_schedule](./pic/stack_status/pid0_3.png)

这里之所以有省略号同样是因为函数内可能有局部变量或者内存对齐等原因，比较重要的是我画出了从0号进程开始运行my_process到调用my_schedule()时两个函数的Prolog,即
```
    pushl %ebp
    movl %esp, %ebp
```
这两句执行后的结果，因为这里形成的ebp硬链表对于理解栈的切换十分重要。
+ 当进入my_process后，Prolog在栈中放入指向kernel[即my_start_kernel]栈存放old ebp的当前ebp值，然后修改ebp指向old ebp pushed by my_process
+ 进入my_schedule后，prolog在栈中放入指向my old ebp pushed by my_process的当前ebp值，然后修改ebp指向 old ebp pushed by my_schedule
**如果0号和1号进程是可以返回的，那么ebp将顺着这个硬链表回溯到my_start_kernel运行时的状态。**

下面继续追踪紧接着的汇编代码运行后的栈变化：
```
	"pushl %%ebp\n\t" /* save ebp */
	"movl %%esp,%0\n\t" /* save esp */
````
首先在0号进程栈中保存当前的ebp，然后将此时的esp值存入task[0].thread.sp，即保存0号进程的运行栈现场。此时栈中情况如图：

![pid 0 stack when save env](./pic/stack_status/pid0_4.png)

```
	"movl %2,%%esp\n\t" /* restore esp */
	"movl %2,%%ebp\n\t" /* restore ebp */
```
+ 开始进行栈切换，即从task[0].stack，切换到task[1].stack，和切换到0号进程相同，修改esp指向task[1].stack[KERNEL_STACK_SIZE - 1]
+ 同样，由于此时1号进程并没有运行，所以ebp作为栈基址，指向地址和esp相同
此时栈情况如下图：

![pid 1 stack switch](./pic/stack_status/pid1_0.png)

	"movl $1f,%1\n\t" /* save eip */

这里的$1f是个“Magic Number”，其效果是把一个指向if分支中的如下指令所在内存
```
"1:\t" /* next process start here */
"popl %%ebp\n\t"
```
的值送入task[0].thread.ip，其效果是当下一次进程切换回0号进程时，将会从这里继续执行。
接下来的

	"pushl %3\n\t"

把1号进程的进程入口地址** [这里以因为是1号进程第一次运行，所以就是my_process()的入口地址]**入栈。此时栈中情况如图：

![pid 1 stack when start proc](./pic/stack_status/pid1_1.png)

	"ret\n\t" /* restore eip */
从当前栈task[1].stack中弹出刚刚送入的1号进程的入口地址，启动1号进程，进入1号进程后，同样有如下动作，当1号进程的my_process()方法内的局部变量i满足if语句条件后，将检查是否需要调度，这里我们同样假设调度标志已经为真，且经过sand_priority()方法后下一个将被运行的进程为0号进程，则进入my_schedule()开始调度。
当执行玩my_schedule()的Prolog后，运行栈情况如图：

![pid 1 stack when schedule](./pic/stack_status/pid1_2.png)


*****


####接下来开始分析从1号进程切换回0号进程的的流程
**由于0号进程之前运行过，所以其状态是runnable，则调度过程中将进入my_schedule()方法的if分支执行**
```
......

    if(next->state == 0)/* -1 unrunnable, 0 runnable, >0 stopped */
    {//save current scene
     /* switch to next process */
     asm volatile(	
         "pushl %%ebp\n\t" /* save ebp */
         "movl %%esp,%0\n\t" /* save esp */
         "movl %2,%%esp\n\t" /* restore esp */
         "movl $1f,%1\n\t" /* save eip */	
         "pushl %3\n\t"
         "ret\n\t" /* restore eip */
         "1:\t" /* next process start here */
         "popl %%ebp\n\t"
         : "=m" (prev->thread.sp),"=m" (prev->thread.ip)
         : "m" (next->thread.sp),"m" (next->thread.ip)
     );
     my_current_task = next;//switch to the next task
     printk(KERN_NOTICE "switch from %d process to %d process\n>>>process %d running!!!<<<\n\n",prev->pid,next->pid,next->pid);
  }

......
```

前两句汇编语句
```
	"pushl %%ebp\n\t" /* save ebp */
    "movl %%esp,%0\n\t" /* save esp */
```
将把当前进程，即1号进程的栈环境保存，具体做法是把当前ebp入栈，然后把栈顶指针esp存入1号进程的task[1].thread.sp，执行后的栈情况如图：

![pid 1 stack after save env](./pic/stack_status/pid1_3.png)

接下来将进行栈切换，这里是从1号进程的进程栈切换到2号进程的进程栈
```
	"movl %2,%%esp\n\t" /* restore esp */
```
此条汇编语句执行后，栈顶所指的栈已经改变，即由1号进程切换到0号进程，如图：

![pid 1 stack after save env](./pic/stack_status/pid1_4.png)

注意红色的esp，这里esp刚好会指向我们在上一次调度时在0号进程栈里放入的old ebp的内存地址，原因很简单，因为上次我们在破坏esp的值切换栈之前保存的esp就是指向这里。

接着将保存向1号进程的task[1].thread.ip里存入合理的值，并且把上次进程切换时保存的0号进程的程序指令指针,task[0].thread.ip入栈，为下一步的切换做最后准备
```
	"movl $1f,%1\n\t" /* save eip */
    "pushl %3\n\t"
```
执行后的栈情况如图：

![pid 0 stack ](./pic/stack_status/pid10_0.png)

这里合理的值就是通过使用“Magic Num” ** $if**，来让task[1].thread.ip指向后面标号**1:**处的指令；同样，在上次从0号进程切换到1号进程时，也有这个操作，所以此时不论task[0].thread.ip还是task[1].thread.ip都是指向标号处的指令，即
```
	"1:\t" /* next process start here */
    "popl %%ebp\n\t"
```
然后由1号进程，即当前进程调用的my_schedule()将执行最后一条指令
```
	"ret\n\t" /* restore eip */
```
其后的语句将在下次进行进程切换，切换回1号进程时继续执行。
此时栈情况如图：

![pid 0 stack before start](./pic/stack_status/pid10_1.png)

这里之所以保留了褐色内存区块的内容，即刚刚push入栈的0号进程的程序指令指针，是为了强调**虽然ret指令从栈里弹出数值到eip，并且令栈缩小，但是如果相应的内存区块的值如果一直没有被修改，那么它的值其实还在，只不过由于esp已经回缩，所以程序无法通过正常手段访问了而已**

到这里，我们看到eip和esp都已经切换回了由进程0切换到进程1最后时刻的值，当cpu继续沿着eip指向的指令，继续执行时，其实就是回到了进程0调用的my_schedule()的执行流上，并且即将执行的指令就是，0号进程调用的my_schedule()方法内的下面这条汇编语句
```
	"1:\t" /* next process start here */
    "popl %%ebp\n\t"
```
执行完上述指令后，栈情况如图**注意esp和ebp的变化**：

![pid 0 stack first ins after start](./pic/stack_status/pid10_2.png)

在这之后，0号进程将顺着执行流，完成对my_schedule()的调用，并且返回0号进程的my_process，回到如下的栈状态：

![pid 0 my_schedule return](./pic/stack_status/pid10_3.png)

注意ebp和esp的变化，特别说明的是这里esp指向画这省略号的地方的原因是为了说明my_process方法内有局部变量，esp和ebp之间是有距离的，但是距离的多少这里不去计算，所以这样表达。

通过上述过程看到，程序将回到0号进程的my_process()，接下来的执行过程就会循环刚刚上述分析的执行过程，在满足一定条件后开始新一轮的进程调度，比如再从0号进程切换会1号进程。其过程和上述类似，这里就不再详述。

##总结
通过上述分析，可以看到现代计算机在运行过程中由于要支持多任务，必不可少的会进行进程调度，进程调度中最为奇妙的就是上下文的转换，这里我们看到，**对于mykernel**来说，next和prev之间的角色是相互转换的，上一个next在下一次进程调度的时候可能会变成prev，上一个prev在下一个进程调度的时候就是next。即是一种“参数化上下文转换”的形式。my_schedule方法是调度中的关键，其最奇妙的地方就是其执行总是分为两部分，调度时执行前一部分，下一次调度回来后才会执行剩下的部分。对于现代计算机来说，其多任务运行的方式就是上述过程的复杂化实现。