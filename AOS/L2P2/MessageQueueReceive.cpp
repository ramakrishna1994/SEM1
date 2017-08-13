#include <stdio.h>
#include <string.h>
#include <sys/msg.h>
#include <stdlib.h>

int main()
{
	struct message
	{
		long type;
		char msgbuf[256];
	} msg;

	int msqid;
	int key=1234;
	msqid = msgget(key,IPC_CREAT|0644);
	if(msqid < 0)
	{
		perror("Error in Connecting or Opening the message queue ");
		exit(1);
	}

	int status = msgrcv(msqid,&msg,sizeof(msg.msgbuf),3,IPC_NOWAIT);
	if(status < 0)
	{
		perror("Error in Reading Message from Queue");
		exit(1);
	}

	printf("Messages Read Succesfully!!\n");
	printf("%s",msg.msgbuf);
	return 0;

}
