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
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/wait.h>
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
		pid_t pidr=fork();
		if(pidr==-1){
			perror("fork");
		}
		if(pidr==0){//child proces  子进程负责处理请求
			//cout<<"child process..."<<endl;
			close(sockfd);
			char buf[1024]={0};
			cout<<"client ip: "<<ip<<endl;
			int r=read(cfd,buf,1023);//从客户端读取来命令信息
			pid_t pid=fork();//fork一个子进程去执行命令
			if(pid==-1){
				perror("fork");
				return -1;
			}
			if(pid==0){
				//子子进程
				close(cfd);
				close(sockfd);
				//使用新的映像替换旧的映像
				//execvp("ps",ps_argv);
				//execv("/bin/ps",ps_argv);
				cout<<"准备替换进程啦！！！"<<endl;
				//execl("/bin/ls","ls","-l",NULL);
				//perror("execl");//会执行吗?
				system(strcat(buf," >tt"));
				exit(-1);
			}
			//子进程等子子进程写完文件再从其中读取
			wait(NULL);
			//usleep(200);
			size_t ffd=open("tt",O_RDONLY);
			if(ffd==-1){
				perror("open");
				return -1;
			}	
			cout<<"start read tt..."<<endl;
			while(1){
					cout<<"send client  message..."<<endl;
					memset(buf,0,1024);
					int r =read(ffd,buf,1024);
					if(r<=0)break;
					send(cfd,buf,r,0);
			}			
			close(ffd);
			close(cfd);
			exit(0);
		}
		//father process
		close(cfd);
	}
	close(sockfd);
	return 0;
}
