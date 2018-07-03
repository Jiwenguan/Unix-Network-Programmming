#include <stdio.h>
#include <sys/types.h>         
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

int main(int argc,char *argv[]){
    char *msg="the first net program...\n";
    struct sockaddr_in serv;//ipv4地址家族类型的变量
    //创建一个通讯端点
    int fd=socket(AF_INET,SOCK_STREAM,0);
    if(fd==-1){
        perror("socket");
        return -1;
    }
    //初始化服务器的ip地址和端口号
    serv.sin_family=AF_INET;//使用的地址家族
    serv.sin_port=htons(6666);    //端口号
    //ip地址 127.0.0.1
    inet_pton(AF_INET,argv[1],&serv.sin_addr);

    //向服务器发起连接,就要知道服务器的ip地址和端口号
    int c=connect(fd,(struct sockaddr *)&serv,sizeof(serv));
    if(c==-1){
        perror("connect");
        return -1;
    }
    // 向服务器发送消息
    write(fd,msg,strlen(msg));
    //阻塞等待服务器的响应消息
    char buf[128];
    int r=read(fd,buf,128);
    //将服务器的响应消息输出到显示器
    write(1,buf,r);
    //关闭fd
    close(fd);
    return 0;
}
