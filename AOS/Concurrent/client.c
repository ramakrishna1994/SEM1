#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include "dictionary.h"


CLIENT *cl;
input in;
output out;
int choice;



void insert();
void getmeaning();
void deletemeaning();
char** str_split(char* , const char);


int main(int argc,char **argv)
{

		if(argc!=2){
			printf("client localhost");
			exit(1);
		}

		cl = clnt_create(argv[1],DICTIONARY_PROG,DICTIONARY_VERS,"tcp");
		while(1)
				{

					printf("----------------------------------------------\n");
					printf("Enter \n1 to Insert new word and\n2 to Get meaning\n3 to Delete Word: ");
					scanf("%d",&choice);

					printf("----------------------------------------------\n");
					switch(choice)
					{
						case 1 :
							printf("Enter Word to add to dictionary: ");
							scanf("%s",in.word);
							getchar();
							printf("Enter Meaning : ");
							scanf("%[^\n]s",in.meaning);
							insert();
							break;
						case 2 :
							printf("Enter word to get meaning from dictionary: ");
							scanf("%s",in.word);
							getmeaning();
							break;
						case 3 :
							printf("Enter Word to delete from dictionary : ");
							scanf("%s",in.word);
							getmeaning();
							deletemeaning();
							break;
						default :
							printf("Enter correct option\n");
							break;

					}
				}

}


void insert()
{
	insert_proc_1(&in,&out,cl);
	if(out.status == 1)
		printf("Word and its meaning inserted successfully to database!!\n");
	else if(out.status == 2)
		printf("Word and its meaning already exists in dictionary\n");
	else
		printf("Error in inserting the word to database!!\n");

}


void getmeaning()
{
	getmeaning_proc_1(&in,&out,cl);
	if(out.status == 1)
	{
		printf("meaning(s) of \"%s\" : \n",out.word);
		char **meanings = str_split(out.allmeaningsappended,',');
		for (int i = 0; meanings[i]; i++)
		{
			printf("%d) %s\n",i+1, meanings[i]);
		}
	}
	else
		printf("Word doesn't exists in Dictionary!!\n");

}

void deletemeaning()
{
	delete_proc_1(&in,&out,cl);
	if(out.status == 1)
		printf("Word successfully deleted from Dictionary!!\n");
	else
		printf("Word not found in Dictionary!!\n");

}


char** str_split(char* a_str, const char a_delim)
{
    char** result    = 0;
    size_t count     = 0;
    char* tmp        = a_str;
    char* last_comma = 0;
    char delim[2];
    delim[0] = a_delim;
    delim[1] = 0;

    /* Count how many elements will be extracted. */
    while (*tmp)
    {
        if (a_delim == *tmp)
        {
            count++;
            last_comma = tmp;
        }
        tmp++;
    }

    /* Add space for trailing token. */
    count += last_comma < (a_str + strlen(a_str) - 1);

    /* Add space for terminating null string so caller
       knows where the list of returned strings ends. */
    count++;

    result = malloc(sizeof(char*) * count);

    if (result)
    {
        size_t idx  = 0;
        char* token = strtok(a_str, delim);

        while (token)
        {
            assert(idx < count);
            *(result + idx++) = strdup(token);
            token = strtok(0, delim);
        }
        assert(idx == count - 1);
        *(result + idx) = 0;
    }

    return result;
}


