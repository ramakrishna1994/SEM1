#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "../Assignment/dictionary.h"



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



int main(int argc,char **argv)
{
	CLIENT *cl;
	common_in in;
	insert_out *insert_output;
	getmeaning_out *getmeaning_output;
	delete_out *delete_output;
	if(argc!=2){
		printf("client localhost");
		exit(1);
	}

	cl = clnt_create(argv[1],DICTIONARY_PROG,DICTIONARY_VERS,"tcp");
		int choice;

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
					printf("Enter Meaning : ");
					scanf("%s",in.meaning);
					insert_output = insert_proc_1(&in,cl);
					if(insert_output->iswordinserted)
						printf("Word inserted successfully to database!!\n");
					break;
				case 2 :
					printf("Enter word to get meaning from dictionary: ");
					scanf("%s",in.word);
					getmeaning_output = getmeaning_proc_1(&in,cl);

					if(getmeaning_output->iswordpresent == 1)
					{
						printf("meaning(s) of \"%s\" : \n",getmeaning_output->word);
						char **meanings = str_split(getmeaning_output->meaning,',');
						for (int i = 0; meanings[i]; i++)
						{
							printf("%d). %s\n",i+1, meanings[i]);
						}
					}

					else
						printf("Word doesn't exists in Dictionary!!\n");
					break;
				case 3 :
					printf("Enter Word to delete from dictionary : ");
					scanf("%s",in.word);
					delete_output = delete_proc_1(&in,cl);
					if(delete_output->isworddeleted == 1)
						printf("Word successfully deleted from Dictionary!!\n");
					else
						printf("Word not found in Dictionary!!\n");
					break;
				default :
					printf("Enter correct option\n");
					break;

			}
		}

	return 0;

}
