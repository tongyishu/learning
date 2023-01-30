## 涉及头文件
#include <openssl/ssl.h>

#include <openssl/err.h>

 

## 常用函数
### SSL库的初始化
使用OpenSSL之前，必须对SSL协议进行初始化。
int SSL_library_int(void);

### SSL协议选择
利用 OpenSSL 开始 SSL 会话之前，需要为客户端和服务器制定本次会话采用的协议，目前能够使用的协议包括TLSv1.0、TLSv1.2、TLSv1.3、SSLv2、SSLv3、SSLv2/v3。推荐使用TLSv1.2，稳定且比较安全。
需要注意的是，客户端和服务器必须使用相互兼容的协议，否则 SSL 会话将无法正常进行。

### SSL会话环境创建
在 OpenSSL 中创建的 SSL 会话环境称为 CTX，申请会话环境的函数：
SSL_CTX *SSL_CTX_new(SSL_METHOD * method);

当 SSL 会话环境申请成功后，还要根据实际的需要设置 CTX 的属性，通常的设置是指定 SSL 握手阶段证书的验证方式和加载自己的证书。指定证书验证方式：
int SSL_CTX_set_verify(SSL_CTX *ctx,int mode,int(*verify_callback),int(X509_STORE_CTX *));
- SSL_VERIFY_NONE 表示不验证
- SSL_VERIFY_PEER 用于客户端时要求服务器必须提供证书，用于服务器时服务器会发出证书请求消息要求客户端提供证书，但是客户端也可以不提供
- SSL_VERIGY_FAIL_IF_NO_PEER_CERT只适用于服务器且必须提供证书，必须与 SSL_VERIFY_PEER 一起使用

为SSL会话环境加载 CA 证书
SSL_CTX_load_verify_location(SSL_CTX *ctx,const char *Cafile,const char *Capath);

从指定文件中加载 CA 证书
STACK_OF(X509_NAME) *SSL_load_client_CA_file(const char *file);

发送 CAs 到客户端，客户端必须提供一个有 CAs 签名的身份证书用于服务端验证客户端
void SSL_CTX_set_client_CA_list(SSL_CTX *ctx, STACK_OF(X509_NAME) *list);

双向认证时，服务端验证客户端
SSL_CTX_set_client_CA_list(ctx, SSL_load_client_CA_file())

SSL 会话加载用户证书：
SSL_CTX_use_certificate_file(SSL_CTX *ctx, const char *file,int type);

SSL 会话加载用户私钥
SSL_CTX_use_PrivateKey_file(SSL_CTX *ctx,const char* file,int type);

验证加载的证书和私钥是否匹配
int SSL_CTX_check_private_key(SSL_CTX *ctx);

### 建立 SSL 套接字
SSL 套接字是建立在普通的 TCP 套接字基础之上，在建立 SSL 套接字时可以使用下面的一些函数：

申请一个SSL套接字
SSL *SSl_new(SSL_CTX *ctx);

绑定读写套接字
int SSL_set_fd(SSL *ssl,int fd);

绑定只读套接字
int SSL_set_rfd(SSL *ssl,int fd);

绑定只写套接字
int SSL_set_wfd(SSL *ssl,int fd);

### 完成 SSL 握手
在成功创建 SSL 套接字后，客户端应使用函数SSL_connect替代传统的函数connect来完成握手过程：

int SSL_connect(SSL *ssl);
而对服务器来讲，则应使用函数SSL_ accept替代传统的函数accept来完成握手过程：

int SSL_accept(SSL *ssl);
握手过程完成之后，通常需要询问通信双方的证书信息，以便进行相应的验证，这可以借助于下面的函数来实现：

X509 *SSL_get_peer_certificate(SSL *ssl);
该函数可以从 SSL 套接字中提取对方的证书信息，这些信息已经被 SSL 验证过了。

X509_NAME *X509_get_subject_name(X509 *a);
该函数得到证书所用者的名字。

### 进行数据传输
当SSL握手完成之后，就可以进行安全的数据传输了，在数据传输阶段，需要使用SSL_read和SSL_write来替代传统的read和write函数，来完成对套接字的读写操作：
int SSL_read(SSL *ssl,void *buf,int num);
int SSL_write(SSL *ssl,const void *buf,int num);

### 关闭 SSL 通信
当客户端和服务器之间的数据通信完成之后，调用下面的函数来释放已经申请的 SSL 资源：

关闭SSL套接字
int SSL_shutdown(SSL *ssl);

释放SSL套接字
void SSl_free(SSL *ssl);

释放SSL会话环境
void SSL_CTX_free(SSL_CTX *ctx);
