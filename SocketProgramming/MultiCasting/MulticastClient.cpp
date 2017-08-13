#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

int main()
{
	int sockfd,buf[256],portno;
	char mip[32];
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
	printf("Enter Data : ");
	scanf("%s",buf);
	if(sendto(sockfd,buf,sizeof(buf),0,(struct sockaddr *)&mcast,sizeof(mcast)) < 0)
	{
		perror("Error in Sending Data ");
		close(sockfd);
		exit(1);
	}
	return 0;
}
