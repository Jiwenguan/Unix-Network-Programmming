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
#include<unistd.h>
#include<string.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<fstream>
#include<string.h>
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
	addrC.sin_port=8080;
	inet_pton(AF_INET,"127.0.0.1",&(addrC.sin_addr.s_addr));
	//connect
	int c = connect(fd,(SA*)&addrC,sizeof(addrC));
	if(c==-1){
		perror("connect");
	}
	char buff[1024]={0};//cin>>;
	char f[1024]={0};
	while(1){
		scanf("%s",buff);
		if(!strncmp(buff,"exit",4)){
			break;
		}	
		write(fd,buff,strlen(buff));
		
		read(fd,f,1023);
		if(strncmp(f,"file",4)){
			break;
		}
		
		ofstream ofs(buff,ios::out);
		while(1){
			int r =read(fd,f,1023);
			if(r<=0)break;
			ofs.write(f,1023);
		}
		cout<<"文件传输完成"<<endl;
		ofs.close();
		
	}
	close(fd);
	return 0;
}



















