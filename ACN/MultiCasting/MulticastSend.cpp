#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>

int main()
{
	int sockfd,portno;
	char mip[32],buf[256];
	struct sockaddr_in mcast;
	printf("Enter Multicast IP address to Send Data :");
	scanf("%s",mip);
	printf("Enter Multicast Port : ");
	scanf("%d",&portno);
	sockfd = socket(AF_INET,SOCK_DGRAM,0);
	if(sockfd < 0)
	{
		perror("Error in Opening Socket");
	}
	memset(&mcast,'\0',sizeof(mcast));
	mcast.sin_addr.s_addr = inet_addr(mip);
	mcast.sin_family = AF_INET;
	mcast.sin_port = htons(portno);
	while(1)
	{
		printf("Enter Data : ");
		gets(buf);
		if(sendto(sockfd,buf,strlen(buf)+1,0,(struct sockaddr *)&mcast,sizeof(mcast)) < 0)
		{
			perror("Error in Sending Data ");
			close(sockfd);
			exit(1);
		}
		printf("Data Sent Successfully!!\n");
	}

	return 0;
}
