/* Display all network interface names */
#include <linux/netlink.h>   //sockaddr_nl
#include <linux/rtnetlink.h> //rtgenmsg, ifinfomsg
#include <stdio.h>           //printf, perror
#include <stdlib.h>          //exit
#include <string.h>          //memset, strlen
#include <sys/socket.h>      //msghdr
#include <unistd.h>          //close

#define BUFSIZE 8192

void rtnl_print_link(struct nlmsghdr *h)
{
    struct ifinfomsg *iface = NLMSG_DATA(h);
    int len = RTM_PAYLOAD(h);

    for (struct rtattr *attr = IFLA_RTA(iface); RTA_OK(attr, len); attr = RTA_NEXT(attr, len))
    {
        switch (attr->rta_type)
        {
        case IFLA_IFNAME:
            printf("Interface %d : %s\n", iface->ifi_index, (char *)RTA_DATA(attr));
            break;
        default:
            break;
        }
    }
}

int main(void)
{
    struct sockaddr_nl sock = {0};
    sock.nl_family = AF_NETLINK;
    sock.nl_groups = 0;

    int fd = socket(AF_NETLINK, SOCK_RAW, NETLINK_ROUTE);
    if (fd < 0)
    {
        perror("socket");
        exit(1);
    }

    struct nlmsghdr hdr = {0};
    hdr.nlmsg_len = NLMSG_LENGTH(sizeof(struct rtgenmsg));
    hdr.nlmsg_type = RTM_GETLINK;
    hdr.nlmsg_flags = NLM_F_REQUEST | NLM_F_DUMP;
    hdr.nlmsg_seq = 1;
    hdr.nlmsg_pid = getpid();

    struct iovec io = {0};
    io.iov_base = &hdr;
    io.iov_len = hdr.nlmsg_len;

    struct msghdr msg = {0};
    msg.msg_iov = &io;
    msg.msg_iovlen = 1;
    msg.msg_name = &sock;
    msg.msg_namelen = sizeof(sock);

    /* sendmsg: 1 nlmsghdr, RTM_GETLINK
        +-------------------------------+
        | struct nlmsghdr |   payload   |
        +-------------------------------+
    */
    if (sendmsg(fd, &msg, 0) < 0)
    {
        perror("sendmsg");
        exit(1);
    }

    int done = 0;
    char buf[BUFSIZE];
    while (done == 0)
    {
        (void)memset(buf, 0, sizeof(buf));
        msg.msg_iov->iov_base = buf;
        msg.msg_iov->iov_len = sizeof(buf);

        int len = recvmsg(fd, &msg, 0);
        if (len < 0)
        {
            perror("recvmsg");
            exit(1);
        }
        /* recvmsg: N nlmsghdr, 0 ~ N-2 RTM_NEWLINK, N-1 NLMSG_DONE
            +--------------------------------+--------------------------------+---------------+
            | struct nlmsghdr |   payload    | struct nlmsghdr |   payload    |     ...       |
            +--------------------------------+--------------------------------+---------------+
                              <- NLMSG_DATA ->                 <- NLMSG_DATA ->
            <------------------------------ N (struct nlmsghdr + payload) -------------------->
        */
        for (struct nlmsghdr *mp = (struct nlmsghdr *)buf; NLMSG_OK(mp, len); mp = NLMSG_NEXT(mp, len))
        {
            switch (mp->nlmsg_type)
            {
            case NLMSG_DONE:
                done++;
                break;
            case RTM_NEWLINK:
                rtnl_print_link(mp);
                break;
            default:
                printf("Ignored msg: type=%d, len=%d\n", mp->nlmsg_type, mp->nlmsg_len);
                break;
            }
        }
    }

    close(fd);
    return 0;
}


/*
宏NLMSG_ALIGN(len)用于得到不小于len且字节对齐的最小数值
#define NLMSG_ALIGNTO 4U
#define NLMSG_ALIGN(len) (((len)+NLMSG_ALIGNTO-1) & ~(NLMSG_ALIGNTO-1))

宏NLMSG_HDRLEN用于计算nlmsghdr消息头的长度
#define NLMSG_HDRLEN ((int) NLMSG_ALIGN(sizeof(struct nlmsghdr)))

宏NLMSG_LENGTH(len)用于计算数据部分长度为len时实际的消息长度
#define NLMSG_LENGTH(len) ((len) + NLMSG_HDRLEN)

宏NLMSG_SPACE(len)返回不小于NLMSG_LENGTH(len)且字节对齐的最小数值
#define NLMSG_SPACE(len) NLMSG_ALIGN(NLMSG_LENGTH(len))

宏NLMSG_DATA(nlh)用于取得消息的数据部分的首地址
#define NLMSG_DATA(nlh) ((void*)(((char*)nlh) + NLMSG_LENGTH(0)))

宏NLMSG_NEXT(nlh,len)用于得到下一个消息的首地址，同时len也减少为剩余消息的总长度
#define NLMSG_NEXT(nlh,len) ((len) -= NLMSG_ALIGN((nlh)->nlmsg_len), (struct nlmsghdr*)(((char*)(nlh)) + NLMSG_ALIGN((nlh)->nlmsg_len)))

宏NLMSG_OK(nlh,len)用于判断消息是否有len这么长
#define NLMSG_OK(nlh,len) ((len) >= (int)sizeof(struct nlmsghdr) && (nlh)->nlmsg_len >= sizeof(struct nlmsghdr) && (nlh)->nlmsg_len <= (len))

宏NLMSG_PAYLOAD(nlh,len)用于返回payload的长度
#define NLMSG_PAYLOAD(nlh,len) ((nlh)->nlmsg_len - NLMSG_SPACE((len)))
*/
