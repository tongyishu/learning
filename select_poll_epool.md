

#### pthread_once

```cpp
#include <pthread.h>

int pthread_once(pthread_once_t *once_control, void (*init_routine)(void));

pthread_once_t once_control = PTHREAD_ONCE_INIT;
```

pthread_once可以保证在多线程的环境中init_routine初始化函数执行且仅执行一次，常用于lib全局变量的初始化以及单例模式编程。



#### select

```c
#include <sys/select.h>

int select (int n, fd_set *readfds, fd_set *writefds, fd_set *exceptfds, struct timeval *timeout);
```

select 函数监视的文件描述符分3类，分别是writefds、readfds、和exceptfds。
调用后select函数会阻塞，直到有描述副就绪（有数据可读、可写、或者except），或者超时（timeout指定等待时间，如果立即返回设为null即可），函数返回。当select函数返回后，可以通过遍历fdset，找到就绪的描述符。

select目前几乎在所有的平台上支持，其良好跨平台支持也是它的一个优点。
select的一 个缺点在于单个进程能够监视的文件描述符的数量存在最大限制，在Linux上一般为1024，可以通过修改宏定义甚至重新编译内核的方式提升这一限制，但是这样也会造成效率的降低。



#### poll

```c
#include <poll.h>

int poll (struct pollfd *fds, unsigned int nfds, int timeout);

struct pollfd {
	int fd;
	short events;
	short revents;
};
```

fds：是一个struct pollfd结构类型的数组，用于存放需要检测其状态的socket描述符。
nfds：用于标记数组fds中的结构体元素的总数量 。
timeout：poll函数调用阻塞的时间（单位毫秒）。

select和poll都需要在返回后，通过遍历文件描述符来获取已经就绪的socket。事实上，同时连接的大量客户端在一时刻可能只有很少的处于就绪状态，因此随着监视的描述符数量的增长，其效率也会线性下降。 



#### epoll

```c
#include <sys/epoll.h>

int epoll_create(int size)；

int epoll_ctl(int epfd, int op, int fd, struct epoll_event *event)；

int epoll_wait(int epfd, struct epoll_event * events, int maxevents, int timeout);

struct epoll_event {
  __uint32_t events;
  epoll_data_t data;
};

typedef union epoll_data
{
  void *ptr;
  int fd;
  uint32_t u32;
  uint64_t u64;
} epoll_data_t;
```

epoll使用一个文件描述符管理多个描述符。

epoll_create创建一个epoll的句柄，size用来告诉内核这个监听的数目的大小，size并没有限制epoll所能监听的描述符最大个数，只是对内核初始分配内部数据结构的一个建议。 

epoll_ctl函数是对指定描述符fd执行op操作。 

- epfd：epoll_create返回的监听fd。
- op：表示op操作，用三个宏来表示：添加EPOLL_CTL_ADD，删除EPOLL_CTL_DEL，修改EPOLL_CTL_MOD。分别添加、删除和修改对fd的监听事件。
- fd：是被监听的fd。
- epoll_event：是告诉内核需要监听的事件。

epoll_wait等待epfd上的io事件，返回需要处理的事件数目。参数events用来从内核得到事件的集合，maxevents告之内核这个events有多大，这个maxevents的值不能大于创建epoll_create()时的size。参数timeout是超时时间（单位毫秒，0会立即返回，-1将不确定）。
