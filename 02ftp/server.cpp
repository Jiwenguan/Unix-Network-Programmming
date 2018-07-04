//*******************************************************
//* @ author :Jiwenguan 
//* @ date   :2018-07-03
//* @ brief  :multi-process fundemantal-TCP-Programming
//*
//*******************************************************
#include<stdio.h>
#include<iostream>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<errno.h>
#include<unistd.h>
#include<string.h>
#include<stdlib.h>
#include<sys/types.h> 
#include<sys/socket.h>
#include<fstream>
using namespace std;
typedef sockaddr_in SA4;
typedef sockaddr SA;
int main(int argc,char* argv[]){
	//create socket descriptor
	
	int sockfd=socket(AF_INET,SOCK_STREAM,0);//
	
	if(sockfd==-1){
		//print failed message;
		perror("socket");
	}
	//bool bREUSEADDR=true;
	//setsockopt(sockfd,SOL_SOCKET,SO_REUSEADDR,(const char*)&bREUSEADDR,sizeof(bool));
	//prepare address
	SA4 addrS;
	addrS.sin_family=AF_INET;
	addrS.sin_port=8080;
	inet_pton(AF_INET,"127.0.0.1",&(addrS.sin_addr.s_addr));
	//bind sockfd with address
	if(bind(sockfd,(SA*)&addrS,sizeof(addrS))==-1){
		//error
		perror("bind");
		return -1;
	}
	//listen for connection on a socket
	if(listen(sockfd,10)==-1){
		perror("listen");
		return -1;
	}
	while(1){
		//prepare a structure to save client address
		SA4 addrC;
		socklen_t len=sizeof(SA4);
		//accept a request
		int cfd=accept(sockfd,(SA*)&addrC,&len);
		if(cfd==-1){
			perror("accept");	
			continue;
		}
		char *ip=inet_ntoa(addrC.sin_addr);//convert IP address
		pid_t pid=fork();
		if(pid==-1){
			perror("fork");
			return -1;
		}
		if(pid==0){//child process
			//cout<<"child process..."<<endl;
			close(sockfd);
			char buf[1024]={0};
			cout<<"client ip: "<<ip<<endl;
			//接收客户端发来的文件请求，文件名称不超过1023个字符
			ssize_t rlen = recv (cfd, buf,sizeof (buf) - sizeof (buf[0]), 0);
			buf[rlen]='\0';
			//打开该文件读取其中数据。
			ifstream ifs(buf,ios::in|ios::binary);
			if(ifs==0){
				cout<<"open failed..."<<endl;
				char f[]="no such file";
				send(cfd,f,strlen(f),0);
				return -1;
			}
			char f[]="file transmission...";
			send(cfd,f,strlen(f),0);
			char p[1024]={0};
			while(1){
				//读取1023个字节数据到p中
				ifs.read(p,1023);
				send(cfd,p,1023,0);//向文件描述符发送数据
				if(ifs.eof()){
					cout<<"文件传输完毕"<<endl;
					ifs.close();
				}
			
			}
			close(cfd);
			exit(0);
		}
		//father process
		close(cfd);
	}
	close(sockfd);
	return 0;
}
