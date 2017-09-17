#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <string.h>
int main()
{
	int sockfd,portno;
	char buf[256],ip[32];
	struct sockaddr_in serveraddr;
	printf("Enter the IP address of Server:");
	scanf("%s",ip);
	printf("Enter the port:");
	scanf("%d",&portno);
	sockfd = socket(AF_INET,SOCK_DGRAM,0);
	if(sockfd < 0)
	{
		printf("Error in Opening Socket");
		exit(1);
	}
	memset(&serveraddr,'\0',sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = inet_addr(ip);
	serveraddr.sin_port = htons(portno);
	while(1)
	{
		printf("Enter the Data :");
		scanf("%s",buf);
		if(sendto(sockfd,buf,sizeof(buf),0,(struct sockaddr *)&serveraddr,sizeof(serveraddr)) < 0)
		{
			printf("Error in Sending Data");
			exit(1);
		}
	}

	return 0;
}
