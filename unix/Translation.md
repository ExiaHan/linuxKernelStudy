#The Unix Time Sharing System
##Translation by ExiaHan
######翻译还有许多不足之处，见谅

##什么是Unix
Unix是为大型数字设备PDP-11和Interdata 8/32计算机设计的一个多用途、多用户的交互式操作系统。它提供了大量即使是在大型操作系统上也很少有的功能，包括：

+ 可卸载卷的分层文件系统
+ 兼容文件、设备和进程间I/O
+ 启动异步进程的能力
+ 以用户为单位的系统命令语言选择
+ 超过100个子系统包含十几种编程语言语言
+ 高度可移植性

##一、简介

到1978这篇论文发布，已经有四个版本的Unix分时操作系统，早期版本[1969-1970]运行在数字设备PDP-7和PDP-9计算机上。第二版的Unix运行在PDP-11/20计算机上。第三版合并了多任务功能，运行在PDP-11/34,/40,/45,/60和/70计算机上，在此之前发表的一篇论文上描述的就是第三版Unix，也是目前使用最广泛的版本。本文仅仅描述第四版，也即当前版本，运行在PDP-11/70和Interdata 8/32计算机上。实际上，不同的Unix版本之间的差异并不大；大部分的修订源于发表这篇论文，撇开这些特定风格，必须要做的是详细解释文件系统的实现。

自从1971年2月份PDP-11 Unix操作系统开始投入使用以来，超过600台装有Unix的设备已经投入使用并提供服务。其中大部分被用于运行和计算机科学教育等相关的应用程序、一些文本资料的格式化处理、收集和处理Bell System内部一些交换设备的问题数据，以及记录和核实电话服务订单。

对于我们自己而言，更多的是把Unix用于操作系统，程序语言，计算机网络和其他计算机科学领域的研究工作，以及文档处理。

对于Unix来说，其最重要的价值在于展示了一个强大而不昂贵的交互式操作系统：Unix可以运行在小于40000美元的设备上，每年小于两个人力资源的系统维护。不管怎样，我们希望用户能发现Unix最大的特点在于简单，优雅，易于使用。

除了一些操作系统应用的功能外，其他的一些Unix支持的应用包括：

+ C编译器
+ 基于QED的文本编辑器
+ 汇编器、连接加载器、符号调试程序
+ 照相排版和方程设定程序
+ 十几种编程序员的支持，包含Fortran 77，Basic， Snobol， APL， Algol 68， M6， TMG， Pascal。

Unix系统中还包含许多的维护管理、实用、娱乐和一些新奇的程序，它们都是使用本地化语言编写。在Unix社区上，成千上万的会员还为Unix提供了更多的程序和语言。值得注意的是Unix是完全自我支持的，所有的Unix程序都由Unix自身维护。同样，这篇论文和其他的一些文档都是在Unix上使用Unix提供的编辑器和排版工具生成。

##二、硬件和软件环境

用来作为研究目的的Unix系统安装在PDP-11/70设备中，该设备是一个16位（即8位一个字节）计算机，带有768KB核心内存，系统内核占据90KB的空间，并且被代码和数据表平分。这个版本的Unix系统，还包含大量的设备驱动，并且为I/O缓存和系统表分配了大量空间；连同内核在一起，运行上文所述的软件所需要的内存最小是96KB。此外还有一些大型设备安装了Unix，作为实例，可以参考PWB/UNIX系统的描述。同样，也有一些用来安装在有限制条件的设备上的版本。

我们的PDP-11带有两块200MB的活动磁头磁盘用作存储和交换。有20个变速接口连接到速率为300-1200波特的数据集，还有12个硬件接口连接到速率为9600波特的一些终端和卫星计算机。此外还有一些速率为2400-4800波特的同步通讯接口用做机器之间的文件传输。最后，还有各种各样的外设包括久轨磁带，一台线性打印机，一台声音合成器，一台照相排字机，一个数字交换网络和一台奕棋机。

Uinx软件的优势在于它们是使用上面提到的C语言编写而成。在Uinx的早期版本上，一些软件的早期版本使用汇编语言编写，但是从1973年的夏天开始，它们全部由C语言实现。这些软件的大小相比于之前来说稍微大了一点。但是由于新版的Unix不仅更容易理解和定制，而且还包含了很多功能性改进，包括多道程序和可以在多个用户程序中共用可重入代码的功能，我们认为这中大小的增长是合适的。

##三、文件系统

作为操作系统，其最重要的角色就是提供一个文件系统。从用户的角度来看，一共有三种文件类型：普通磁盘文件，目录和特殊文件。

####普通文件

一个文件包含了用户放入其中的全部信息，如符号的或者二进制（目标）程序。Unix系统不做任何特殊的结构化要求。一个文本文件由简单的字符串和用来分行的换行符组成。对于二进制程序文件，当它们进入核心内存开始运行时，是一序列的程序字。一小部分用户程序使用更多的结构来处理文件，比如汇编生成器和装载器会希望一个目标文件以一种特殊的格式组织起来。但是这些文件结构由使用到它们的程序来控制，而不是由操作系统控制。

####目录

目录提供了文件名和文件名代表的文件本身之间的映射，从而从整体上感知文件系统的结构。每个用户拥有一个专门的目录用来存放他自己的文件，此外，他还可以在此目录下创建一些子目录来方便的一组文件归档在一起。一个目录和普通文件的行为表现十分类似，但是目录不能被没有权限的程序改写，所以目录文件的内容有操作系统控制。但是，任何有适当权限的用户或程序都可以像读普通文件一样读取目录文件的内容。

Unix本身维护一些目录供其自己使用。其中一个目录是根目录。系统中的所有文件都可以通过目录链来搜索路径直到找到目标万能键。搜索开始的地方通常是根节点。其他系统目录包含了所有系统提供的通用程序，即所有的系统命令。就像即将看到的，实际上，一个程序是否可以被运行与其是否位于这些目录中无关。

文件可以由不超过14个字符命名。当一个文件的文件名被指定后，它可能会形成一个路径名--一个由一系列目录名并且其文件名结尾，各目录名和文件名之间由斜杠分隔。如果这个路径以斜杠作为开始，则对文件的搜索开始于根目录。例如，路径名 /aplpa/beta/gamma会造成系统从根目录开始，在根目录下搜索alpha，然后在目录alpha下搜索beta，最后在beta目录下找到gamma文件。gamma可能是一个普通文件，或者一个目录，又或者是一个特殊文件。作为特殊情况，文件名“/”代表根目录。

一个不会以“/”开始的路径名会让系统从用户当前所在的目录进行搜索。因此，路径名alpha/beta指出文件beta位于当前目录的子目录alpha中。对于最简单的路径名，比如，aplha，表示一个位于当前目录下的文件。这里另一种特殊情况，空文件名表示当前目录。

相同的非目录文件可以出现不同的目录下而且可能有不同的名称。这个特性叫做链接。一个文件的目录条目偶尔被叫做连接。Unix系统不同于其他系统的地方就在于允许链接，因为所有指向一个文件的链接拥有同等状态。换言之就是，一个文件不存在于一个特殊的目录里；一个文件的目录条目仅仅由它的名字和一个指向实际描述这个文件信息的指针组成。因此一个文件独立存在于任何目录条目中，尽管事实上一个文件是否会消失由最后一个链接到它的链接决定。

每个目录至少有两个条目，路径名 “.”表示这个目录本身。因此一个程序可以通过使用“.”读当前目录的内容而不用知道当前目录的完整路径。路径名“..”被约定为指向当前目录的父目录，即为指向当前目录被创建时所在的目录。

目录结构被限制为拥有一颗有根树的形式，除去特殊条目 “.”和“..”，每个目录只能作为条目明确的出现在其父目录中。这么做的原因是简化访问目录结构的子树的程序的编写，更加重要的是为了避免部分分离的层次结构。如果到任意目录的链接都是允许的，那么当从根节点到一个目录的最后连接被切断时，将很难检测到。

####特殊文件

特殊文件构成Unix文件系统最不同寻常的特征。每个被系统支持的I/O设备被与至少一个特殊文件关联起来。特殊文件可以像普通磁盘文件一样读写，但是对特殊文件的特写请求最终会导致激活特殊文件对应设备。特殊文件条目位于目录“/dev”下，尽管这些特殊文件可能有位于其他普通目录的链接。因此，举例来说，对一个磁带的写操作可能是对特殊文件“/dev/mt”的进行写操作。每条通信线，每块磁盘，每个磁带机，物理主存，都有特殊文件与之对应。当然，当前活动的磁盘和内存特殊文件是被保护起来的，以防止不加选择的访问。

以这种方式处理I/O设备将带来三重优势：文件和I/O设备在用户看起来尽可能的相似;文件和设备的名称有相同的语法规则和意义，所以一个被程序期望作为参数传入的文件名可以是一个设备名;最后，特殊文件同普通文件具有同样的保护机制。

####可移除文件系统

尽管文件系统的根目录总是存储在相同的设备上，整个文件系统的层次结构都位于该设备上却不是必要的。存在一个挂载系统请求，其要求两个参数：已经存在的一个普通文件的文件名，一个与具有独立文件系统结构并且包含他自己的目录层次的存储卷关联的特殊文件文件名。挂载的结果是使之前提供的普通文件变成与这个可移动卷的上的文件系统根目录相对应。实际上，挂载操作把层次树上的一个叶结点（这里是这个普通文件）替换为一颗子树（存储在可移动卷上的层次结构）。挂载之后，位于可移动卷上的文件和位于常驻磁盘上的文件之间没有实质性区别。举例来说，在我们安装的系统上，根目录位于我们一块磁盘驱动器的一个小分区上，但是其他的存储用户文件的驱动器，是通过系统初始化队列挂载的。一个可挂载文件系统通过对其对应的特殊文件的写操作来生成。一个工具程序可以用来创建一个空的文件系统，或者可以简单的拷贝一个已经存在的文件系统。

对于位于不同设备上的文件处理来说，对文件的操作规则仅有一点例外：不允许存在一个横跨两个文件系统层次之间的链接。这个限制是强制执行的，用以避免当可移动卷被卸载时出现的复杂链接记账问题，保证当可移动卷被卸载时，所有与其相关的链接也都被移除。

####保护

尽管访问控制方案非常简单，它却有一些不同寻常的特性。系统中的每个用户都被赋予了一个唯一的用户标识数。当一个文件被创建时，会打上其所有者的用户标识。同样也会给予一个新文件的是一个10位的保户位。其中的9位独立的指出对于文件所有者，本组其成员和所有剩余用户对该文件的读、写和执行权。

如果第十位被置位，当这个文件被当作程序执行时，系统会临时把用户标识由当前用户改为改文件创建者的用户标识。这种用户标识上的改变仅发生在当该文件被当作程序执行时。这个设置用户标识的特性为程序提供了使用其他用户无法访问的文件的权利。例如，一个程序可能拥有一个既不应该被该程序自身读或者改变账单文件。如果这个程序的设置用户标识位被置位，它可以访问这个文件，尽管这次的访问对于其他由程序提供者发起的程序来说是禁止的。因为调用程序的实际用户标识是一直可用的，设置用户标识的程序可以采取各种措施按照它们调用者的凭证来满足它们自己的需求。这种机制被用来允许用户执行一些需要谨慎执行的调用系统特权条目的写操作命令。举例来说，有一个系统条目仅仅能被超级用户调用，创建一个空目录。就像上面已经指出的，目录要有指向“.”，“..”的条目。这个创建目录的命令是由超级用户所有并且其设置用户标识位被置位。当它检查它的调用者的认证权限创建指定指定的目录后，它创建并且为“.”，“..”创建条目。

因为任何人都可能把他所有的文件的设置用户标识位置位，这个机制通常在没有管理的也是可用的。举例来说，这个保护机制简单的解决了由Apeph-null提出的“MOO计数问题”。

系统识别一个特殊的用户标识（超级用户）并豁免其在文件访问上的一般限制。因此，程序可以被编写用来在不受保护系统干扰的情况下转储和重载文件系统。

####I/O调用

用来进行I/O的系统调用被设计出来的目的是消除各个设备之间的不同和各式各样的访问方式。在随机和线性I/O之间没有区别，同样，任何由系统利用的逻辑记录大小也不区分。一个普通文件的大小取决于其中写入了多少字节的数据，预先决定一个文件的大小是不必要的，同时也是不可能的。

为了区分I/O的要素，下文中以一种匿名语言并且指出需要的参数以避免陷入复杂的底层关系来总结一些基础调用。每个调用都可能返回错误，用来简单表示不位于调用队列的表述。

要读或者写一个假定已经存在文件，必须用如下调用打开
```bash
	filep = open(name, flag)
```
name指明文件名，也可以是任意的路径名。flag参数指明这个文件是已读是写还是更新，或者是读写。

返回值filep叫做一个文件描述符。它是一个小整数用来在随后的读、写、或者其他操作调用中标识这个文件。

要创建一个新文件或者完全的重写一个旧文件，有一个create系统调用用来在指定文件不存在的时候创建它，或者在指定文件存在的时候把文件长度截断为0。create也打开这个新文件用来进行写操作，就像open一样，它返回一个文件描述符。

文件系统的维护对用户没有任何可见的锁，对一个文件有多少用户可以拥有对其进行读写的权限也没有限制。尽管可能出现一个文件被两个用户争夺使用同时向其写入数据的情况，在实践中问题没有出现。我们认为锁既没有必要也是不适合在我们的环境中来阻止用户同时访问同一个文件。他们是不必要的是因为我们没有去面对一个由独立进程们维护的足够大的，单文件的数据。它们是不适合的因为在常见的锁场景里，当一个用户正在读文件是，通过阻止另一个用户进行写文件操作并不能阻止出现文件内容的混乱，举例来说，两个用户都在编辑同一个文件会导致两个用户编辑的实际上是文件的一个副本。

也就是，不管怎样，当两个用户同时写同一个文件、在同一目录创建文件、或者删除其他用户打开的文件时，会使用合适的内部锁来维护文件系统的一致性。

除了下面说明的，读和写是顺序的。这意味着如果一个特定的字节是文件中最后一个被读出或者写入的，那么下次的I/O调用隐含的从这个字节的下一个字节开始读写。对于每个打开的文件，都有一个指针，指针由系统维护，指明了下一个将被读写的字节。如果n个bytes被读写，那么指针前进n字节。

打开一个文件时，下面的调用可能会用到：
```bash
	n = read(filep, buffer, count)
    n = write(filep, buffer, count)
```
最多有count指明的字节数在filep指定的文件和buffer之间传输。返回值n是实际传输的字节数。在写调用中，n和count除了一些异常情况，如I/O错误或者到达文件结尾外，两者是相等的。对于read调用，除了出现错误，n通常小于count。如果读指针非常接近文件结尾，那么读count个字节会导致读越界，此时只有合适的字节，即到文件结尾的字节被传输；同样，像终端这样的打字式输入其，永远不会返回超过一行的输入。当读调用返回0时，说明已经到文件尾。对于磁盘文件来说，这会让读指针和当前文件大小相等。如果设备支持的话，通过在终端输入Esc键，可以生成一个EOF。

多少字节被写入仅仅影响文件写指针和count指定的部分，其他部分不会被改变。如果写入的最后一个字节超过当前文件大小，那么文件大小会增加。

如果要随机[直接访问]进行I/O操作，只要把读写指针移动到需要的文件位置就好。
```bash
	location = lseek(filep, offset, base)
```
和filep相关的指针会移动到距离文件开头、或者从当前位置、又或者从文件结尾，offset字节的地方，到底从哪里开始做计算偏移和基地址有关。便宜可以是负数。对于一些设备来说，比如纸带机和终端，seek调用会被忽略。函数返回从文件开头到当前指针所在的实际偏移。

有很多附加的系统I/O调用这里不会被讨论，比如关闭文件，获取文件转该，改变文件保护字或者改变文件所有者，创建一个目录，创建一个指向已存在文件的链接，删除一个文件。

##四、文件系统的实现

像在3.2节中提到的，一个目录条目仅仅包含了一个文件名和指向该文件本身的指针。这个指针是一个整数，叫做文件索引数i-number[index-number]。当这个文件被访问时，它的i-number被用来当作一个索引，从已知该目录被存放的存储位置的系统表[i-list]中找到其inode。inode中包括：

+ 所有者的uid，gid
+ 保护位信息
+ 物理磁盘和磁带上存放文件内容的地址
+ 文件大小
+ 创建时间，最后使用时间和最后修改时间
+ 到该文件的链接数，也即其出现在目录中的次数
+ 一个代码说明文件是目录、普通文件还是特殊文件

open和create系统调用的目的是把用户给的路径通过搜索用户显示指定或者系统默认的目录转换为i-number。当一个文件被打开时，它的设备，i-number和读写指针被存放在一个系统表中，其所在位置通过open或者create返回的文件表述符[file descriptor]进行索引。因此，当一个随后的调用来读写这个文件时，这个文件描述符可以很容易的找到要访问这个文件所需的必要信息。

当一个新文件被创建是，会为其分配一个i-node节点，同时一个目录条目会被创建，用来存放文件名和其文件指针。对一个已存在文件创建链接牵涉已新的名字创建一个目录条目、拷贝i-number，增加i-node节点中link-count域的值。如果link-count数降为0,那么这个文件所在的磁盘块被释放，其inode节点也被回收。

磁盘上的所有空间在其所包含的文件系统中被分成一连串512bytes的块，块在逻辑上被编址，地址从0到设备的上限。对每个文件的inode节点来说，其中能存放13个设备地址。对非特殊文件，开头10个设备地址指向文件的开头10个块。如果文件大小超过10个块，第11个设备地址指向一个间接块，其中包含了128个额外的块地址，用来指向多出的部分。如果文件大到额外的128个地址也无法表示完，那么第12个设备地址指向一个双重间接快，其中包含128个地址指针，每个指针又指向一个包含128个设备块地址的数据块。当然，如果依然有需求的话，第13个设备块将会是一个三重间接块。如此依赖，一个文件最大可以有（10+128+128×128+128×128×128）×512=1082201088bytes，大概1G。一旦文件被打开，小于5120字节的数据可以直接通过单磁盘访问来读取;在5120到70656的字节需要两次访问；70575到8459264的字节需要三次访问；更大的如1082201088字节的需要四次访问。实际上，设备缓存机制被证明可以消除多数通过间接访问获取文件内容。

上述讨论适用于普通文件。当到一个文件的I/O请求被创建时，如果它的inode说明它是特殊文件，那么最后12个设备地址字非实质的，并且第一个指明一个内部设备名，通过使用一对数字来表示，分别表示设备类型和子设备数。设备类型说明哪个系统程序会被用来处理这个设备的I/O；子设备数用来选择对应子设备，如当一个磁盘被附加到一个特殊的控制器或者多个类似终端接口的其中一个时。

在这个环境上，mount系统调用的实现非常直接。mount维护一个系统表，其每一表项的内容是i-number和调用mount是传入的普通文件的设备名，其对应的值是指定的特殊文件的设备名。在open或create调用时，这张表用来根据给出的路径名来搜索其中的i-number/device对来转换。

对用户来说，读写文件是同步且无缓冲的。这意味着read一旦返回，那么数据就可用;同样，在写操作之后用户的工作空间可以重复使用。事实上，系统问呼了一个非常合适的缓冲机制来减少访问一个文件的I/O的次数。比如一个Write调用每次只传输1字节。系统将会搜索他的缓存去确定被影响的磁盘块当前是不是在主存里，如果不是，把它从设备读入内存。然后受影响的字节在缓存里被第换，并且一个条目会被创建说明有多少块要被写。然后产生Write的返回值，尽管这时候可能还没有完成I/O。相反的，如果一个单字节被读，系统会判断该字节所在的二级存储块是否已经位于系统的某个缓存里，如果是，那么可以立马返回这个字节的值，否则，读入其所在块到缓存，然后拿到这个字节值。

当系统识别到一个程序正在顺序访问一个文件时，会异步预读下一块。通过稍微增加的一点点系统开销，显著的减小了程序运行时间。

一个程序一次读写512字节相比较一次仅读写一个字节来说有一个优势，但是优势不是巨大的；主要来自于避免的系统开销。如果一个程序没有或者很少有大卷的I/O，那么它完全有理由以尽可能小的单元进行读写。

i-list的概念是Unix的一个不同寻常的特性。实际上，这种管理文件系统的方法被证明是非常可靠而且易于处理的。对于系统本身来说，它的一个长处是每个文件都拥有一个短小，清晰的名字来和其保护、寻址、以及其他访问文件所需信息相关连。同时也允许一个非常简单和快速的算法来检测一个文件系统的组成。举例来说，验证每个设备其中的一部分包含有用的信息和设备上那些可以被分配的不相交的空间和用尽的空间。这个算法是独立于目录层次的，因为它仅需要扫描这个线性组织的i-list。同时i-list的概念引出了很多其他文件系统组织里没有的特性。比如，一个问题就是谁应该负责负责一个文件占有的空间，因为一个文件的所有目录条目都有相同的状态。让文件的所有者负责是不公平的实际上，因为一个用户可以创建一个文件，另一个用户可以链接到这个文件，并且第一个用户可能删除这个文件。第一个用户依然是这个文件的所有者，但是现在这个文件应该由第二个用户负责。这个简单合理公平的算法会在拥有到这个文件的用户之间传播应付的责任。很多安装安装版都通过不要求任何费用避免这个问题。

##五、进程和镜像

一个镜像是一个计算机执行环境，包含一个内存镜像，通用寄存器的值的集合，打开的所有文件的状态，当前目录等信息。一个镜像相当于一个仿真计算机的当前状态。

一个进程是一个镜像的执行。当处理器执行一个进程时，镜像必须存在于主存中;在其他进程被执行时，除非有活动的高特权的进程强制把它的镜像交换到磁盘，否则该进程镜像依然保留在主存中。

一个镜像的用户可用内存被分为3个逻辑段。程序的代码段从虚拟地址空间地址0开始。在执行时，这个段是写保护的并且多个执行该程序的进程共享统一份代码段副本。紧接着代码段之上的是不能共享的，可写的数据段，它的大小可以通过系统调用扩展。虚拟地址空间的高地址开始的是一个栈，它随着栈顶指针的浮动向下增长。

####进程

除了系统自我引导的时候，一个新的进程通过系统调用fork创建。
```
	processid = fork()
```
当fork被执行时，当前进程分为独立的两个正在执行的进程。两者有独立的普通内存镜像拷贝，共享所有打开的文件。新进程和原进程唯一不同的地方在于其中一个进程会被认为是另一个的父进程：在父进程里，返回值procesid是子进程的进程标识符，在子进程内其为0。

因为fork在父进程和子进程内的返回值是不同的， 所以可以通过fork的返回值判断哪个是父进程，哪个是子进程。

####管道

相关的进程间同样可以通过使用read和write进行通信。调用：
```bash
	filep = pipe()
```
返回一个文件描述符并且创建一个叫做管道的进程间通道。这个管道就像其他打开的文件一样，在fork时会从父进程传递到子进程。使用管道的读操作会一直等待知道其他进程发生使用管道的写操作。这样，数据就能在这个镜像的两个进程间传递。

尽管通过管道在两个进程之间通信十分有用，但是它不是一种通用机制，因为管道必须由两个进程的共同祖先设置起来。

####进程的执行

另一个主要的系统原语是通过
```
	execute(file, arg1, arg2,..., argn)
```
来调用，用来请求系统读入并且执行通过文件名file指定的程序，并且给他传入一些字符串参数。调用execute的进程的所有的代码和数据都会被file文件中对应的内容替代，但是它打开的文件、当前目录和进程间的关系却不会被改变。除非调用失败，比如无法找到file指定的文件或者文件的执行位未置位，原始的返回会发生在execute执行结束。execute和子程序调用比较起来，更像是一个跳转指令。

####进程同步

另一个进程控制系统调用：
```bash
	processid = wait(status)
```
会造成它的调用者挂起执行直到它的一个子进程完成执行。然后wait返回结束进程的进程ID。如果调用它的进程没有子进程，将返回错误。大部分来自子进程的状态在这里一样可用。

####中止

最后：
```bash
	exit(status)
```
中止一个进程，摧毁它的镜像，关闭它打开的文件，一般还会消除它。父进程可以通过wait原语收到提醒，并且获得status。进程也可能因为一些非法操作或者用户产生的信号中止。

##六、SHELL

对大部分用户来说，通过一个叫做SHELL的程序来与系统交流。Shellshingle命令行解释器：它按行读入用户的输入并且把它们解释成执行其他程序的请求。（Shell在其他地方已经有过完整描述，这里仅仅讨论它的实现理论）。在最简单的情形下，一个命令行由“命令名”和跟在它后面的几个传给这个命令的参数组成，彼此之间用空格分开。
```bash
	command arg1 arg2...argn
```
Shell把命令名和参数列分成独立的字符串。然后去寻找指定名陈的文件；命令可能是一个包含‘/’的路径名，指向系统中的任意一个文件。如果找到了命令，那么它被装入内存并且执行。由Shell搜集的参数对于命令来说是可读取的。当命令执行完毕，Shelll恢复它自己的执行，并且通过一个提示字符指明它已经准备好接受下一个命令。

如果命令文件无法被找到，Shell通常会在command前加上‘/bin/’并且再次尝试查找文件。目录‘/bin’包含的命令是一些常用指令。（用来搜索的目录队列可以被用户修改。）

####标准输入输出

在上面第三节对输入输出的讨论看起来已经包含了一个程序一定要通过打开或者创建获取对应文件的文件描述符的所有文件。被Shell执行的程序，实际上，在执行时默认有三个打开文件，对应的文件描述符是0，1，2。当一个程序被Shell执行时，文件‘1’被打开用来写，即通常理解的标准输出文件。除了下面指明的一些情况，这个文件就是用户的终端。因此程序通常使用文件描述符‘1’指明的文件来写入一些有用的信息。相反的，文件‘0’被打开用来读，程序通过读该文件来获取用户输入。

Shell可以改变这些同用户的终端输出和键盘相关的标准输入输出关联。如果传给命令的一个参数有前缀‘>’，那么对于这个命令来说，文件描述符‘1’将会和‘>’之后的参数指定的文件相关联。举个例子：
```bash
	ls
```
通常在写入的文件描述符上，列出当前目录下的所有文件的名字。命令：
```bash
	ls > there
```
创建一个名叫there的文件然后把这个列表放入there文件中。因此参数 ‘>there’意味着“把输出放入文件there”。另一方面：
```bash
	ed
```
通常会进入编辑器，它会从用户的键盘获取请求。命令
```bash
	ed <script
```
把script解释成editor命令的一个文件，因此 “< scrpit”意味着“从script里获取输入”。

尽管跟在“<”或者“>”后面的文件名看起来会成为命令的一个参数，实际上它完全是被Shell解释的并且不会被传给命令。因此在各个命令里不需要特殊的代码来控制输入输出重定向。命令仅需在适当的时候使用标准文件描述符‘0’和‘1’即可。

文件描述符‘2’，像‘1’一样，通常也会与终端的输出流关联。当一个用“>”说明的输出转移请求被指明后，‘2’仍然会与终端绑定，所以命令可以产生一些不应该写到输出文件里的诊断信息。

####过滤器

一个标准输入输出的扩展概念是直接把一个命令的输出作为另一个命令的输入。一串用垂直线条分隔开的命令组会让Shell同时执行所有的命令，并且把队列里前一个命令的标准输出作为另一个命令的标准输入。因此命令行：
```bash
	ls | pr -2 | opr
```
ls列出当前目录的文件的文件名；它的输出被传给pr，一个会把它的输出加上时间戳来编页（参数 “-2”要求双列输出）。同样的，pr的输出是opr的输入；这个命令把它的输入排入一个文件用来离线打印。

这个步骤可以被用一种更加笨拙的方法完成：
```bash
	ls > temp1
    pr -2 < temp1 >temp2
    opr <temp2
```
通过一些可移除的临时文件。缺乏重定位输入输出的能力，一个依然笨拙的方法可以被用来要求ls命令接受用户的请求来把它的输出编页，来打印成多行格式，然后管理它的输出并送到离线设备。实际上令人惊讶的是，事实上因为一些效率上的问题，期待像ls这样的命令的作者提供一个具有多中输出参数的版本是不明智的。

一个像pr的程序把它们的标准输入拷贝到它的标准输出（带有一些处理）被叫做过滤器。一些过滤器我们已经发现的在字符转写、根据模式从多行中选择、对输入排序和加密解密上有很有用的表现。

####命令分隔符;多任务

另一个Shell提供的特性是相对简单的。命令不需要在不同行，取而代之的是可以通过分号分隔：
```bash
	ls;ed
```
首先会列出当前目录的内容，然后进入编辑器。

一个相关的特性更加有趣。如果过一个命令后面跟有符号“&”，Shell将不会等到命令完成再显示提示符，相反，它立马就能接受一个新命令。举个例子：
```bash
	as source >output &
```
会让source被汇编，带有一些诊断信息可能被输出，不管汇编会花费多长时间，Shell立马返回。当Shell不等到命令完成时，一个标识这个正在运行命令的进程的数字会被打印出来。这个标识符可以用来等待这个命令的完成或者中止它。符号“&”可以再一行中使用多次：
```bash
	as source >output & ls >files &
```
将会把汇编和列出操作都移到后台进行。在这些例子里，一个不同于终端提供的输出文件;如果这个没有被完成，多个命令的输出可能会掺杂在一起。

Shell还允许在上述操作中出现括号，举例来说：
```bash
	(data;ls) >x &
```
把当前时间和紧随其后的当前目录内容的列表写入到文件‘x’里。Shell同时也会立即返回来接收其他的请求。

####Shell是一个命令；命令文件

Shell本身也是一个命令，也看可以被递归调用。假设文件tryout包含这些行：
```bash
	as source
    mv a.out testprog
    testprog
```
mv命令把a.out文件重命名为testprog。a.out是（二进制）汇编其的输出文件，可以被执行。因此如果这三行在键盘上被键入，source将会被汇编，生成的程序将会被重命名为testprog，并且testprog会被执行。当这些行在文件tryout里时，命令：
```bash
	sh <tryout
```
会让shell“sh”序列的执行这些命令。

Shell还有一些其他的能力，包括替换参数和从一个指定在某个目录下的文件里的子集里构造参数链。Shell还提供条件和循环结构。

####Shell的实现

关于Shell的操作的概述现在可以被理解了。大多数是键，Shell在等待用户输入一个命令。当标志这一行结束的换行符出现后，Shell的read调用返回。Shell分析命令行，把参数们一种对execute恰当的方式组织起来。然后fork被调用。子进程的代码依然是这个Shell，尝试去用这些参数执行execute。如果成功，将会开始执行这个给定名字的程序。同时，其他从fork返回的进程，即父进程，等待这个子进程死亡。当它发生时，Shell就会知道命令已经完成，然后Shell显示它的提示符，从键盘读取其他命令。

有了这个框架，后台进程的实现是微不足道的，当一个带有“&”的命令被执行是，Shell仅仅避免它自己等待它创建的用来执行这个命令的进程结束。

愉快的是，所有这些机制与标准输入输入文件的概念协调的十分出色。当一个进程通过fork原语创建后，它不止集成它的父进程的内存镜像，还有说有父进程当前打开的文件，包含文件描述符“0”，“1”，“2”。Shell，当然，用这些文件去读取命令行和写一些提示和诊断信息，并且在一般情况下，它的孩子----命令程序，会自动的继承它们。当一个带有“>”，“<”的参数被给予是，不管怎样，它的子孙，在执行execute前，把它的标准输入输出文件描述符同这些指定名字的文件相关联。这是简单的，因为通过协议，最小的没有用的文件描述符和一个被打开或者创建的新文件关联起来；这仅仅需要关闭文件“0”或者“1”，然后打开这个指定名字的文件。因为运行命令所在的进程在它完结时会简单的结束，这些通过“>”，“<”在文件描述符“0”，“1”之间建立的关联会在进程结束时自动中止。因此，Shell不需要知道它自己的标准输入输出文件的确切名称，因为它不需要重新打开它们。

过滤器是简单的标准输入输出重定向的扩展，它们使用管道取代文件。

在通常环境下，Shell的主循环不会结束。（主循环包含了从父进程fork返回的分支;这表示，这个分支会等待子进程结束，然后读取其他命令行。）唯一会导致Shell中止的是发现在它的输入文件里发现文件结束条件。因此，当一个Shell被当作命令来执行，并且有一个给定的输入文件，就像：
```bash
	sh <comfile
```
在文件comfile里的命令会被执行，直到到达comfile的文件结尾；然后这个通过sh启用的Shell会中止。因此，这个Shell进程是其他Shell进程的子进程，wait在最后将会返回，然后其他的命令可以接着被执行。

####初始化

用户用来输入命令的Shell实例他们自己是其他进程的子进程。系统初始化的最后一步是创建一个单进程和通过execute启用一个程序序，叫做init。init的角色是为每个终端通道创建一个进程。为了给载体建立拨号线路，这些init的子实例打开恰当的终端用来在文件“0”，“1”，“2”上输入输出和等待。然后一个消息会被打印出来要求用户登录。当用户键入一个名字或者其他标识符，恰当的init实例被唤醒，接受输入行，然后读取一个密码文件。如果用户的名字被找到，并且他也提供了正确的密码，init进入用户的默认当前目录，设置这个进程的用户ID为登录用户的ID，然后通过调用一个会执行Shell的execute。这时，Shell就准备好接受命令并且登录协议完成。

同时，init（是所有在之后将成为Shell的init子实例的父母）的的主流路径执行wait。如果有一个孩子进程中止，要么是Shell发现了一个文件结尾或者用户输入了错误的用户名或密码，这个路径下的init简单的重新创建这个死进程，即重新打开恰当的输入输出文件并且打印另一个登录消息。因此用户可以通过简单的输入文件结尾序列给Shell来执行登出操作。

####用其他程序代替Shell

上面描述的Shell是设计用来让用户能够访问系统所有的功能，因为它会以恰当保护模式启动任一程序。有时候，会需要不同的系统交互接口，这种特性可以很容易实现。

想起之前说的在用户输入名字和密码成功登录之后，init通常启动Shell来解释命令行。在密码文件里的用户条目可以包含在登录之后要被启动的用来替代Shell的程序的名字。这个程序可以以任意它想要的方式解释用户的消息。

举例来说，一个秘书编辑系统的密码文件的用户条目里可能会指定用编辑器程序“ed”来替代Shell。这样当用户登录时，他们就会进入编辑器并且立马可以开始工作。同样，在他们尝试唤醒不适用于他们的程序时会被阻止。在实践中，临时退出编辑器来执行格式化软件和其他工具程序。

一些系统中可用的游戏（如国际象棋，黑杰克）指明了一个及其严格的执行环境。对于它们中的任一个在password文件中都有一个条目，指明了取代Shell要被启动的恰当的游戏程序。作为这些游戏的玩家登录的用户会发现他们自己被限制在游戏环境里，不能研究整个Unix（好象很有趣）。

##陷阱

PDP-11硬件能检测一系列的程序故障，如关联到不存在的内存，没有被实现的指令，在期望偶数地址的地方使用了奇数地址等。这些故障导致处理器陷入一个系统例程。除非事先有其他的准备，否则一个非法动作将导致系统中止这个进程并且把它的镜像写到当前目录中的core文件中。一个调试器可以用来决定程序故障时的状态。

那些不断循环产生不需要的输出的程序，或者那些用户有其他想法的程序可以通过使用中断信号来停止，具体是键入“delete”键。除非已经采取了特殊的动作，否则这个信号简单的导致程序停止执行，并且不会有core文件产生。此外还有一个quit信号用来强制产生一个镜像文件。因此那些执行不期望的循环的程序可以被中止并且剩下的不会被预先检查。

硬件产生的错误，中断和quit信号，可以被一个进程忽略和捕捉。举例来说，Shell忽略quit信号来阻止已登录的用户退出。编辑器捕捉终端并且返回它的命令级别。这在不丢失工作进度的情况下停止一个长时间打印时是有用的（编辑器维护一个它正在编辑的文件的副本）。在一个没有浮点硬件的系统中，没有被实现的指定将被捕获，浮点指令将被中断。

##愿景

也许有点自相矛盾，Unix的成功很大程度上源于它不用设计用来适用与任何预先指定的对象。当我们中的一个（汤姆森）写好第一个版本时，由于不满意可用的计算机的功能，发现了一个少有任用的PDP-7，并且开始创建一个更加友好舒适的环境。这种（特别是个人）努力成功充分的得到了其他作者和一些同事的兴趣，最后证明使用PDP-11/20,特别是去支持一个文本编辑器和排版系统。当在PDP-11/20充分开发后，这个系统被证明是十分有用的，而且足够说服管理投资PDP-11/45,最后是在PDP-11/70和Interdata 8/32机器上，一直到它发展到现在的情况。我们的目标是贯彻努力，像绞链一样，一直在系统的探索发现，其他软件与机器的舒适关系上做努力。至今我们还没有去必须面对某一个人的需求，因为有这种自由，我们很开心。

回顾Unix的设计，有三点原则。

第一点：因为我们是程序员，我们自然的会想设计一个系统并且很容易的用它来写，测试，和运行程序。我们对编程便利性的最重要的表达就是系统被安排为交互使用，即使初始版本只支持一个用户。我们相信一个正确设计的交互系统相对于一个批处理系统来说是更加创造性和令人满意的。此外，这样一个系统也可以很容易的适应非交互的使用方式，但是反过来就不行。

第二点：对系统和它的软件大小保持严格的限制。从给定的效率和能效相互钳制的角度来看，对大小的限制不仅在经济效益上是很优秀的，而且也是一种十分优雅的设计。这看起来可能是“救苦救难”的一种巧妙的伪装，但是在我们这种情况下，它确实起作用了。

第三点：几乎从系统启动开始，系统就可以也确实可以管理它自己（自洽）。这个事实比它看起来的还要重要。如果一个系统的设计者被强迫去使用这个系统，他们很快就会意识到它的功能上和表面上的缺陷并且会有很强的动力去在这些问题变的糟糕前去修复它们。因为说有的源程序总是可以很容易的在线修改，我们希望有其他人可以在有新的想法发现或者提议时对系统进行修改和重写。

这篇关于Unix的文章讨论的方面最主要是这三个设计原则里的前两个。举例来说，对于文件系统的接口，在一个程序的角度上是极度的方便。在较低的接口层面上是设计用来消除不同设备、文件、目录和序列化访问的区别。不需要大型的“访问方法”例程来隔绝程序员与系统调用；实际上，所有的用户程序要么直接调用系统功能，要么使用一个小的库程序，这些库程序的大小都小于一个页，它们缓冲一些字符并且对它们进行一次读写。

另一个重要编程便利性的方面是没有需要文件系统或者其他系统调用分开管理和控制的复杂“控制块”结构。一般来说，一个进程的地址空间内的内容是只属于这个程序的，我们试图避免在地址空间内的数据结构的放置限制。

考虑到所有程序都应该可以使用任意文件或者设备作为输入和输出文件，所以可考虑到把设备依赖原则放入了系统本身。唯一的替代选择看起来是对于所有程序都加载所有处理各个设备的例程，但是会造成昂贵的设备开销，或者会依赖于在恰当的时候动态链接恰当的设备服务例程，即会造成昂贵的开销。

同样的，进程控制策略和命令行接口已经被证明是便利和高效的。因为Shell像一个普通的可交换的用户程序一样运行，它不消耗系统的被联系的固有空间，并且它应该是被设计成最小开销但是十分有用的。特别的，考虑到Shell是像一个进程一样执行并且孵化其他进程来执行命令的框架，I/O重定向的概念、后台进程，命令文件和用户可选的系统接口的实现是微不足道的。

####影响

Unix的成功很大程度上不是依赖新的发明，而是从已有的丰富的想法中慎重选择了一部分并做了充分开发，特别是在某些方面它们可以看成是实现一个小型的功能强大的系统的关键点。

fork操作，一个我们实现的必需品，是在GENIE分时操作系统里出现的。在很多观点上我们还收到了Multics的影响，我们借鉴了它I/O系统调用的特殊方式还有Shell这个名称以及它的一些常用功能。Shell应该为每条命令创建一个进程的概念也是借鉴了Multics，尽管最后Multics这个系统因为效率问题被搁置了。此外一个类似的策略也被TENEX使用。

##统计

接下来的数字是推荐的用来研究Unix操作的规模。我们的没有在文档中涉及到的用户倾向于使用系统来进行软件开发，特别是语言工作。还有一小部分重要的“应用”程序。

所有在一起，今天我们有：

+ 125个用户
+ 33的最大同时用户数
+ 1630个目录数
+ 28,300个文件
+ 301,700个512字节的二级存储块被使用

还有一个“后台”进程运行在可能的低优先级上；它被用来吸收屌CPU的空闲时间。它被用来处理常熟‘e’的百万位近似计算，和其他一些半无限的问题。如果算上这些后台工作，我们每天平均：

+ 13,500条命令
+ 9.6小时的CPU时间
+ 230小时的连接时间
+ 62个不同用户
+ 240次登录

##感谢

对于Unix的贡献者，实际上有太多的人需要被提及。当然，要向我们在计算科学研究中心的同事致敬。R.H.Canaday为基础的文件系统设计贡献了很多。我们特别要感谢R.Morris，M.D.Mcllroy和J.F.Ossanna提供的那些创造性点子，深思熟虑的批评和一成不变的支持。

##参考资料

1. L. P. Deutch and B. W. Lampson, "An online editor," Commun. Assn. Comp.Mach., 10 (December 1967), pp. 793-799, 803.

2. B. W. Kernighan and L. L. Cherry, "A System for Typesetting Mathematics,'Commun. Assn. Comp. Mach., 18 (March 1975), pp. 151-157.

3. B. W. Kernighan, M. Ε. Lesk, and J. F. Ossanna, "UNIX Time-Sharing System:Document Preparation," B.S.T.J., this issue, pp. 2115-2135.

4. T. A. Dolotta and J. R. Mashey, "An Introduction to the Programmer's Work­bench," Proc. 2nd Int. Conf. on Software Engineering (October 13-15, 1976),pp. 164-168.

5. T. A. Dolotta, R. C. Height, and J. R. Mashey, "UNIX Time-Sharing System: The Programmer's Workbench," B.S.T.J., this issue, pp. 2177-2200.

6. H. Lycklama, "UNIX Time-Sharing System: UNIX on a Microprocessor," B.S.T.J.,this issue, pp. 2087-2101.

7. B. W. Kernighan and D. M. Ritchie, 77if C Programming Language, Englewood Cliffs, N.J.: Prentice-Hall, 1978.

8. Aleph-null, "Computer Recreations," Software Practice and Experience, / (April-June 1971), pp. 201-204.

9. S. R. Bourne, "UNIX Time-Sharing System: The UNIX Shell," B.S.T.J., this issue,pp. 1971-1990.

10. L. P. Deutch and B. W. Lampson, Doc. 30.10.10, Project genie,. April 1965.

11. R. J. Feiertag and Ε. I. Organick, "The Multics input-output system," Proc. Third Symposium on Operating Systems Principles (October 18-20, 1971), pp. 35-41.

12. D. G. Bobrow, J. D. Burchfiel, D. L. Murphy, and R. S. Tomlinson, "TENEX, a Paged Time Sharing System for the PDP-10," Commun. Assn. Comp. Mach.,15 (March 1972), pp. 135-143.