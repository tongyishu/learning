gdb默认情况下，父进程fork一个子进程，gdb只会继续调试父进程而不会管子进程的运行。

Linux2.5.60之后的版本，可以使用以下的方法来进行多进程的调试。

1. 跟踪子进程进行调试，可以使用set follow-fork-mode xxx来设置fork跟随模式。
1.1 show follow-fork-mode
    进入gdb以后，我们可以使用show follow-fork-mode来查看目前的跟踪模式。

1.2 set follow-fork-mode parent
    gdb只跟踪父进程，不跟踪子进程，这是默认的模式。

1.3 set follow-fork-mode child
    gdb在子进程产生以后只跟踪子进程，放弃对父进程的跟踪。

2. 同时调试父进程和子进程，Linux提供了set detach-on-fork xxx命令来供我们使用
2.1 show detach-on-fork
    show detach-on-fork显示了目前是的detach-on-fork模式

2.2 set detach-on-fork on
    只调试父进程或子进程的其中一个(根据follow-fork-mode来决定)，这是默认的模式。

2.3 set detach-on-fork off
    父子进程都在gdb的控制之下，其中一个进程正常调试(根据follow-fork-mode来决定),另一个进程会被设置为暂停状态。
