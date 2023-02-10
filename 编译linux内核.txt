###  编译linux-3.10内核（gcc-4.8.5）
step1：官网下载linux-3.10内核源代码并编译：

tar -xzvf linux-3.10.tar.gz
cd linux-3.10
make mrproper # 清除所有配置及编译残留
cp /boot/config-3.10.0-327.el7.x86_64 .config # 使用当前机器的内核配置，make defconfig使用默认配置
make -j `nproc`

step2：至此linux内核已经编译完成，产生的linux内核文件为arch/x86/boot/bzImage，以下是linux内核的安装部分：
make modules_install # 内核模块安装
cp -v arch/x86/boot/bzImage /boot/vmlinuz-3.10.10-lfs-7.4 # 内核文件安装，Linux系统引擎。
cp -v System.map /boot/System.map-3.10.10 # map文件安装，地址和符号表，映射内核中所有函数和数据结构的入口点和地址
cp -v .config /boot/config-3.10.10 # 配置文件安装，包含内核所有的配置选项

###  基于busybox制作initramfs文件系统（gcc-4.8.5）
step 1：编译busybox（官网下载busybox，静态编译busybox依赖于glibc-static，可使用yum install glibc-static进行安装）
tar -xjvf busybox-1.32.1.tar.bz2
cd busybox-1.32.1
make defconfig # 使用默认配置
sed -i "s/.*CONFIG_STATIC.*/CONFIG_STATIC=y/g" .config # 静态编译busybox
make -j `nproc` && make install

step 2：创建磁盘设备ram和控制台设备console
进入busybox-1.32.1/_install目录，执行以下命令：
mkdir dev
mknod dev/console c 5 1

step 3：创建系统启动的第一个进程文件--init
进入busybox-1.32.1/_install目录，创建init文件并写入以下内容，并使用chmod +x init添加可执行权限：


#!/bin/sh
echo "INIT SCRIPT"
mkdir /proc
mkdir /sys
mount -t proc none /proc
mount -t sysfs none /sys
mkdir /tmp
mount -t tmpfs none /tmp
echo -e "\nThis boot took $(cut -d' ' -f1 /proc/uptime) seconds\n"
exec /bin/sh

step 4：生成cpio.gz压缩文件
find -print0 | cpio --null -ov --format=newc | gzip -9 > initramfs-busybox-x64.cpio.gz

###  启动linux-3.10内核
启动命令：
qemu-system-x86_64 \
-kernel bzImage \
-nographic \
-append "console=ttyS0 nokalsr root=/dev/ram rw init=/init" \
-enable-kvm \
-cpu host \
-initrd initramfs-busybox-x64.cpio.gz \
-m 1024 \
-gdb tcp:1234 # 启动gdb远程调试，端口为1234

参数解释：
-kernel 内核镜像
-nographic 非图形化界面启动
-append 与kernel连用，追加内核的启动参数，内核启动后可通过cat /proc/cmdline查看
-enable-kvm 使能内核的kvm模块
-cpu 指定cpu集
-initrd 指定内核运行的文件系统
-m 指定内存大小
-gdb 是否使用gdb调试

终止运行：
kill -9 `pidof qemu-system-x86_64`

###  gdb调试linux内核
start_kernel()是内核的汇编与Ｃ语言的交接点，在该函数以前内核的代码都是用汇编写的，完成一些最基本的初始化与环境设置工作。

start_kernel就好比c代码中的main函数。

[root@compute-192 os-trial]# gdb
(gdb) file linux-3.10/vmlinux
(gdb) target remote:1234
(gdb) b start_kernel
(gdb) c
