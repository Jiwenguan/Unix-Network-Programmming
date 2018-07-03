#include <stdio.h>
#include <sys/types.h>         
#include <sys/socket.h>
#include <netinet/in.h>
#include <ctype.h>
#include <unistd.h>
#include <arpa/inet.h>

int main(void){
    int conn_fd;//用于和客户端连接的描述符
    char buf[1024];
    struct sockaddr_in serv;//声明一个ipv4地址家族类型的变量,这个变量是具体的通信地址,包括ip地址和端口号.本机的ip地址和端口号

    //创建一个通讯端点,返回一个文件描述符
    int lfd=socket(AF_INET,SOCK_STREAM,0);
    if(lfd==-1){
        perror("socket");
        return -1;
    }
    //需要对serv变量的成员进行初始化
    serv.sin_family=AF_INET;
    serv.sin_port=htons(6666);
    serv.sin_addr.s_addr=htonl(INADDR_ANY);

    //将serv绑定到lfd上.
    int b=bind(lfd,(struct sockaddr *)&serv,sizeof(serv));
    if(b==-1){
        perror("bind");
        return -1;
    }
    //将lfd设置为被动连接,当有连接到来的时候,将连接放入未决连接队列中
    listen(lfd,5);
    while(1){
        //从未决连接队列中取出一个连接,进行具体的和客户端的通讯
        conn_fd=accept(lfd,NULL,NULL);
        if(conn_fd==1){
            perror("accept");
            return -1;
        }
        //获取客户端的请求消息
        int r=read(conn_fd,buf,1024);
        //对buf中的数据进行处理
        int i;
        for(i=0;i<r;i++)
            buf[i]=toupper(buf[i]);

        //将处理结果返回给客户端
        write(conn_fd,buf,r);
        //关闭本次连接
        close(conn_fd);
    }
    close(lfd);
    return 0;
}
