

#### neutron（网络管理）

```bash
neutron net-list                # 列出所有网络
neutron net-create <net-id>     # 创建网络
neutron net-delete <net-id>     # 删除网络

neutron subnet-create <net-id> --name <subnet-name> <ip>/<net-mask>  # 创建子网
```



#### nova（虚机管理）

```bash
nova list                # 列出所有虚机
nova show <server-id>    # 查询指定虚机
nova start <server-id>   # 启动虚机
nova stop  <server-id>   # 停止虚机
nova boot --image <image-id> --flavor <flavor-id> --nic net-id=<net-id> --availability-zone <zone> <vm-name> # boot虚机
nova delete <server-id>  # 删除指定虚机

nova flavor-list                        # 列出所有虚机规格
nova flavor-show <flavor-id>            # 查询指定虚机规格
nova flavor-create <flavor_name> <memory_size> <disk_size> <vcpus> --is-public False  # 创建虚机规格
nova flavor-delete <flavor-id>          # 删除虚机规格

nova service-list                       # 查询所有物理节点
nova interface-list <server-id>         # 列出虚机网卡
nova get-vnc-console <server-id> novnc  # 查看虚机的vnc地址
```



#### glance（镜像管理）

```bash
glance image-create --name <image-name> --container-format <format> --disk-format <format> --file <image-file>  # 创建虚机镜像
glance image-delete <image-id>      # 删除虚机镜像
glance image-list                   # 列出所有虚机镜像
glance image-show <image-id>        # 查询指定虚机镜像
```



#### cinder（存储管理）

```bash
cinder list                        # 列出所有卷
cinder create --name <volume-name> --volume-type <volume-type> <volume-size>  # 创建卷
cinder delete <volume-id>          # 删除卷

cinder type-list                   # 列出所有类型
cinder type-show <type-id>         # 列出指定类型
cinder type-create <name>          # 创建类型
cinder type-delete <type-id>       # 删除类型
```

