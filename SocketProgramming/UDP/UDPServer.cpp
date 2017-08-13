#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <arpa/inet.h>
int main()
{
	int sockfd,portno;
	char buf[256];
	struct sockaddr_in serveraddr;
	printf("Enter Port No for server to run :");
	scanf("%d",&portno);
	sockfd = socket(AF_INET,SOCK_DGRAM,0);
	if(sockfd < 0)
	{
		printf("Error in Opening socket");
		exit(1);
	}
	memset(&serveraddr,'\0',sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = INADDR_ANY;
	serveraddr.sin_port = htons(portno);

	if(bind(sockfd,(struct sockaddr *)&serveraddr,sizeof(serveraddr))<0)
	{
		printf("Error in Binding");
		exit(1);
	}
	while(1)
	{
		if(recvfrom(sockfd,buf,sizeof(buf),0,NULL,0)<0)
		{
			printf("Error In Reading the Data from client");
			exit(1);
		}
		puts(buf);
	}

	return 0;

}
