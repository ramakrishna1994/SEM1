#include<stdio.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<stdlib.h>
#include<string.h>
#include<netinet/in.h>
#include<arpa/inet.h>

int main()
{
	char serveripaddress[32],buffer[256];
	int sockfd,connfd,portno;
	struct sockaddr_in serveraddress;
	printf("Enter Server IP address to connect : ");
	scanf("%s",serveripaddress);
	printf("Enter Port No : ");
	scanf("%d",&portno);
	
	sockfd = socket(AF_INET,SOCK_STREAM,0);
	if(sockfd < 0)
	{
		printf("Error in Opening Socket");
		exit(1);
	}
	memset(&serveraddress,'\0',sizeof(serveraddress));
	serveraddress.sin_family=AF_INET;
	serveraddress.sin_addr.s_addr=inet_addr(serveripaddress);
	serveraddress.sin_port=htons(portno);
	
	if(connect(sockfd,(struct sockaddr *)&serveraddress,sizeof(serveraddress)) < 0)
	{
		printf("Error in Connecting");
		exit(1);
	}
	printf("Successful in Connecting to Server!!\n");
	while(1)
	{
		printf("Please Enter Message to send to Server : ");
		scanf("%s",buffer);
	
		if(send(sockfd,buffer,sizeof(buffer),MSG_DONTWAIT) < 0)
		{
			printf("Writing Data to Socket Failed");
			exit(1);
		}
	}
		
	
	
}
