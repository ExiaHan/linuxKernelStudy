#Stack-Switch 对进程切换的一些理解

韩洋 + 原创作品转载请注明出处 + 《Linux内核分析》MOOC课程 http://mooc.study.163.com/course/USTC-1000029000

*本文为Mooc网相关课程作业，文中可能引用到一些网络资料，在这里对这些资料的原作者提供感谢。*

*****

##实验准备
按照作业页面的说明去Github上mykernel项目按照其说明下载“linux 3.9.4内核源码”，然后clone项目到本地，解压内核，然后对内核打patch
	```$patch p1 < [your patt]/mykernel/mykernel/mykernel_for_linux3.9.4sc.patch```

