//*******************************************************
//* @ author :Jiwenguan 
//* @ date   :2018-07-03
//* @ brief  :multi-process fundemantal-TCP-Programming
//*
//*******************************************************
#include<iostream>
#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>          /* See NOTES */
#include<sys/socket.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<unistd.h>
#include<string.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<fstream>
#include<string.h>
#include<string>
using namespace std;
typedef sockaddr_in SA4;
typedef sockaddr SA;
int main(int argc,char *argv[]){
	int fd=socket(AF_INET,SOCK_STREAM,0);
	if(fd==-1){
		perror("socket");
		return -1;
	}
	//prepare network address
	SA4 addrC;
	addrC.sin_family=AF_INET;
	addrC.sin_port=8000;
	inet_pton(AF_INET,"127.0.0.1",&(addrC.sin_addr.s_addr));
	//connect
	int c = connect(fd,(SA*)&addrC,sizeof(addrC));
	if(c==-1){
		perror("connect");
	}
	char buff[1024]={0};//cin>>;
	char f[1024]={0};
	while(1){
		cout<<"请输入要下载到本地的文件路径,以回车结束,输exit 退出"<<endl;
		memset(buff,0,1024);
		memset(f,0,1024);
		cin>>buff;
		if(!strncmp(buff,"exit",4)){
			cout<<"退出传输。。。"<<endl;
			break;
		}	
		write(fd,buff,strlen(buff));
		int m=read(fd,f,20);
		cout<<f<<endl;
		f[m]='\0';		
		if(strncmp(f,"file",4)){//如果没有这个文件
			cout<<"远程没有该文件"<<endl;
			continue;
		}
		//将文件路径中名字提取出来,
		string tmp(buff);
		size_t pos = tmp.rfind('/');
		string store;
		if(pos!=-1){
			store = tmp.substr(pos+1);
		}else{
			store=tmp;
		}
		
		cout<<"您的文件将存储在 "<<store<<" 中"<<endl;
				
		size_t ffd=open(store.c_str(),O_RDWR|O_CREAT|O_TRUNC,S_IRWXU);
		if(ffd==-1){
			perror("open");
		}	
		while(1){
			memset(f,0,1024);
			int r=read(fd,f,sizeof(f)-sizeof(f[0]));
			if(r <1023){
				write(ffd,f,r);
				cout<<"读取结束。。。"<<endl;
				break;
			}
			write(ffd,f,r);
		}




		/*ofstream ofs(store.c_str(),ios::out);
		while(1){
			int r =read(fd,f,sizeof(f)-sizeof(f[0]));
			f[r]='\0';
			cout<<f<<"1"<<endl;
			if(r<=0)break;
			ofs.write(f,r);
		}*/
		cout<<"文件传输完成"<<endl;
		close(ffd);
		//ofs.close();
		
	}
	close(fd);
	return 0;
}



















