elf（可执行文件）文件so的使用方式有两种：
1、在elf文件编译时使用-l或-L选项直接链接这个so，又称隐性调用，常用于应用程序。
2、在elf文件执行时使用dlopen函数动态打开这个so，又称显性调用，常用于驱动程序。
隐性调用是比较常见的方式，可以通过ldd命令查看elf文件运行时依赖的so。需要提前将so拷贝到相关目录下，不管程序是否真正使用该so，执行前都会检查该so是否存在，如果检测不到该so则报错。程序一旦执行，就会一刀切地把so全读到内存中，而不管在运行过程中是否会执行到该so。
显性调用是一种插件的思想，依赖于libdl库，无法通过ldd命令查看依赖。如果程序没有运行到dlopen，是不需要拷贝so到相关目录下的，只有在执行到dlopen时，才会检查该so。程序开始运行后，不会立刻读取so到内存中，而是直到执行dlopen时，才会将so的内容动态拷贝到内存中。

这里着重介绍显性调用，即以dlopen的形式使用so。
相关API?
#include <dlfcn.h>

/* 加载指定路径的SO，成功返回so动态库的句柄，失败返回NULL 
filename: so路径
flag: 
    RTLD_LAZY：延迟绑定，仅在执行引用它们的代码时解析符号。如果从未引用该符号，则永远不会解析它。（只对函数的引用执行延迟绑定;在加载共享对象时，对变量的引用总是立即绑定）
    RTLD_NOW：立即绑定，dlopen()返回之前，将解析共享对象中的所有未定义符号。如果无法执行此操作，则会返回错误。
    RTLD_GLOBAL：此共享对象定义的符号将可用于后续加载的共享对象的符号解析。
    RTLD_LOCAL：这与RTLD_GLOBAL相反，如果未指定任何标志，则为默认值。此共享对象中定义的符号不可用于解析后续加载的共享对象中的引用。
    RTLD_NODELETE：在dlclose()期间不要卸载共享对象。如果稍后使用dlopen()重新加载对象，不会重新初始化对象的静态变量。
    RTLD_NOLOAD：不要加载共享对象。可用于测试对象是否已经驻留（yes返回对象的句柄，no返回NULL）。
*/
void *dlopen(const char *filename, int flag);

/* 返回出现的错误 */
char *dlerror(void);

/* 提取so动态库中的函数，返回函数的指针 
handle: dlopen打开的so库的句柄
symbol: 提取的函数名
*/
void *dlsym(void *handle, const char *symbol);

/* 关闭已打开的so动态库 */
int dlclose(void *handle);

dlopen使用示例：
