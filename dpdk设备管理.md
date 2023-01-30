rte_bus_list
全局变量，所有bus的集合，链表形式，一个结点表示一个bus，如pci，vdev。

rte_eth_devices
全局变量，所有eth设备的集合，数组形式，一个数组元素表示一块网卡 。

eth_dev_shared_data
全局变量，eth设备的数据区域，可在dpdk主/从进程间共享。


devargs_list
全局变量，用户设备参数列表，一个结点表示一个用户设备，rte_eal_init时 -a 选项指定。


rte_bus
结构体，定义了bus总线的通用操作方法，包括scan，probe，plug等。


rte_pci_bus
结构体，包含以下内容：
bus，rte_bus通用结构，定义了bus总线的操作方法：
scan，扫描/sys/bus/pci/devices/目录下的内容，填充rte_pci_device结构，bus的scan模式可分为allow和block两种rte_pci_ignore_device，可对用户设备进行过滤。
probe
......
devices_list，所有的PCI设备，链表形式。
drivers_list，所有的驱动，链表形式。

rte_devargs
结构体, 存放用户设备参数。

eal_option_device_add()
函数，将设备参数（struct device_option）添加至devargs_list。

eal_option_device_parse()
函数，将devargs_list中的设备参数（struct device_option）解析成用户设备（struct rte_devargs），并将用户设备（struct rte_devargs）添加至devargs_list（使用rte_devargs_add()添加），同时将重复的设备参数（struct device_option）从devargs_list中删除。

iova-mode
--iova-mode=pa （physical addressing），设备DMA直接使用物理地址，不需要经过iommu进行翻译。
--iova-mode=va （virtual addressing），设备DMA使用虚拟地址，通过iommu将虚拟地址翻译成物理地址。

rte_flow
rte_flow_attr，属性：流规则的属性，如其方向（入口或出口）和优先级。
rte_flow_item，匹配项（rte_flow_item）：匹配模式的一部分，匹配特定数据包数据或流量属性。（匹配模式为任意数量匹配项的组合）
rte_flow_action，动作：数据包与模式匹配时要执行的操作。
enum rte_flow_item_type，枚举了所有的match模式。
enum rte_flow_action_type，枚举了所有的action操作。

rte_socket_id
返回正在运行的lcore所对应的物理socket_id（numa_id）。
