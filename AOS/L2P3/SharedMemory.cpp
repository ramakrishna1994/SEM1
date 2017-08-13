#include <stdio.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdlib.h>

int main()
{
	key_t key;
	struct shmid_ds shmbuffer; // this is where all metadata is stored like segment size etc.
	printf("Enter Key to create Share Memory ID : ");
	scanf("%d",&key);
	int shmid = shmget(key,27,IPC_CREAT | 0666);
	if(shmid < 0)
	{
		perror("Getting Shared Memory Failed ");
		exit(1);
	}
	printf("Shared Memory ID is : %d\n",shmid);
	char *shm = (char *)shmat(shmid,NULL,0);
	if(shm == NULL)
	{
		perror("Attaching process to Shared Memory Failed");
		exit(1);
	}
	printf ("shared memory attached at address %p\n", shm);

	/* Determine the segment's size. */
	shmctl (shmid, IPC_STAT, &shmbuffer);
	int segment_size = shmbuffer.shm_segsz;
	printf ("segment size: %d\n", segment_size);


	printf("Enter Data to be put in shared memory :");
	scanf("%s",shm);
	printf("Data posted to shared memory!!\n");
	/* Detach the shared memory segment. */
	shmdt (shm);
	printf("Detaching process from shared memory\n");
	printf("Attaching the process again to Existing Shared memory\n");
	char *oldshm = (char *)shmat(shmid,shm,SHM_RDONLY);
	if(oldshm == NULL)
	{
		perror("Attaching process again to existing Shared Memory Failed\n");
		exit(1);
	}
	printf("Data from Shared Memory : %s\n",oldshm);
	printf("Detaching process from shared memory again\n");
	shmdt (oldshm);
	return 0;
}
