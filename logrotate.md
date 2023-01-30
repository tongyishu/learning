#### logrotate

- 一个对日志文件进行转储、压缩及删除的linux工具，防止日志文件过大影响服务器运行。

  

- 使用方法：

  ```bash
  logrotate conf_file.conf
  logrotate -s status_file.status conf_file.conf
  ```

  

- 配置文件示例：

  ```bash
  /var/log/message {
      monthly
      rotate 5
      compress
      delaycompress
      dateext
      dateformat %y%m%d.%s
      missingok
      notifempty
      create 644 root root
      postrotate
          /usr/bin/killall -HUP rsyslogd
      endscript
  }
  ```

  

- 常见配置文件字段解释：

  ```bash
  compress                 通过gzip压缩转储以后的日志
  nocompress               不压缩
  copytruncate             用于还在打开中的日志文件，把当前日志备份并截断
  nocopytruncate           备份日志文件但是不截断
  create mode owner group  转储文件，使用指定的文件模式创建新的日志文件
  nocreate                 不建立新的日志文件
  delaycompress            和compress一起使用时，转储的日志文件到下一次转储时才压缩
  nodelaycompress          覆盖delaycompress选项，转储同时压缩。
  errors address           专储时的错误信息发送到指定的E-mail地址
  ifempty                  即使是空文件也转储，这个是logrotate的缺省选项。
  notifempty               如果是空文件的话，不转储
  mail address             把转储的日志文件发送到指定的E-mail地址
  nomail                   转储时不发送日志文件
  olddir directory         转储后的日志文件放入指定的目录，必须和当前日志文件在同一个文件系统
  noolddir                 转储后的日志文件和当前日志文件放在同一个目录下
  prerotate/endscript      在转储以前需要执行的命令可以放入这个对，这两个关键字必须单独成行
  daily                    指定转储周期为每天
  weekly                   指定转储周期为每周
  monthly                  指定转储周期为每月
  rotate count             指定日志文件删除之前转储的次数，0指没有备份，5指保留5个备份
  size size                当日志文件到达指定的大小时才转储，bytes(缺省)及KB(sizek)或MB(sizem)
  missingok                轮循期间，任何错误将被忽略，例如"文件无法找到"之类的错误。
  ```

  

