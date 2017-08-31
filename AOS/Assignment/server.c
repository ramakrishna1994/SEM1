#include <stdio.h>
#include <stdlib.h>

#include "../Assignment/dictionary.h"

char word[100][20]={'0'};
char meaning[100][3][100]={'0'};
int noofmeaningsforword[100]={ 0 };
int noofkeys=0;
char *allmeanings;

int search(char *searchkey)
{
	for(int i=0;i<noofkeys;i++)
	{
		printf("%s\n",word[i]);
		if(strcmp(searchkey,word[i]) == 0)
		{
			return i;
		}
	}
	return -1;
}

char *appendmeanings(int index)
{
	int len = 0;
	allmeanings = malloc( 500 * sizeof(char) );
	for(int i=0;i<noofmeaningsforword[index];i++)
	{
		for(int j=0;j<strlen(meaning[index][i]);j++)
		{
			allmeanings[len]=meaning[index][i][j];
			len++;
		}
		if(i+1 != noofmeaningsforword[index])
		{
			allmeanings[len]=',';
			len++;
		}

	}
	return allmeanings;
}




insert_out *insert_proc_1_svc(common_in *in,struct svc_req *rqstp)
{
	static insert_out outp;
	int temp;

	int index = search(in->word);
		if(index == -1)
		{
			printf("Key doesn't exists!!\n");
			strcpy(word[noofkeys],in->word);
			strcpy(meaning[noofkeys][noofmeaningsforword[noofkeys]],in->meaning);
			noofmeaningsforword[noofkeys]++;
			noofkeys++;
			outp.iswordinserted = 1;
			printf("Word successfully added to dictionary!!\n");
		}
		else
		{
			printf("Key exists at index %d!!\n",index);
			strcpy(meaning[index][noofmeaningsforword[index]],in->meaning);
			noofmeaningsforword[index]++;
			noofkeys++;
			outp.iswordinserted = 1;

		}

	return (&outp);
}

getmeaning_out *getmeaning_proc_1_svc(common_in *in,struct svc_req *rqstp)
{
	static getmeaning_out outp;

		int index = search(in->word);
		if(index == -1)
		{
			outp.iswordpresent = 0;
			printf("Word successfully added to dictionary!!\n");
		}
		else
		{
			printf("Key exists at index %d!!\n",index);
			outp.iswordpresent = 1;

			strcpy(outp.word ,word[index]);
			for(int i=0;i<noofmeaningsforword[index];i++)
			{
				printf("meaning : %s\n",meaning[index][i]);
			}
			char *allmeanings = appendmeanings(index);
			printf("%s",allmeanings);
			strcpy(outp.meaning ,allmeanings);
			free(allmeanings);

		}

	return (&outp);
}

delete_out *delete_proc_1_svc(common_in *in,struct svc_req *rqstp)
{
	static delete_out outp;

	int index = search(in->word);
		//Todo : add getmeaning function
		if(index == -1)
			{
				printf("Key doesn't exists to delete!!\n");
				outp.isworddeleted = 0;
			}
			else
			{
				printf("Key exists and deleted!!\n");
				word[index][1] = '0'; //
				outp.isworddeleted = 1;
			}

	return (&outp);
}


