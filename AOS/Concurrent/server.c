#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdlib.h>

#include "dictionary.h"

/*
 * Structure for storing meaning of a word
 * */
struct m
{
	char *meaning;
	struct m *ptrtonextmeaning;
};

/*
 * Structure for storing word
 * */
struct w
{
	char *word;
	struct m *ptrtomeaning;

	struct w *ptrtonextword;
};


struct w *head = NULL;

/*
 * function for finding the index(address) of a given word.
 * It is a traversing function
 * Input is word to be searched
 * Output is Address of word structure
 * */
struct w* findindexofword(char *word)
{
	 struct w *temp = head;
	 while(temp)
	 {
		 if(strcmp(temp->word,word) == 0)
			 return temp;

		 temp=temp->ptrtonextword;
	 }
	 return NULL;
}

/*
 * Function used to find address of meaning of a word
 * Input is taken as address of word and the meaning
 * Output is address of meaning of the given word
 *
 */
struct m* findindexofmeaninginaword(struct w *indexofword,char *meaning)
{
	struct m* ptrtomng = indexofword->ptrtomeaning;
	while(ptrtomng)
	{
		if(strcmp(ptrtomng->meaning,meaning) == 0)
		{
			printf("Meaning already exists");
			return ptrtomng;
		}
		ptrtomng = ptrtomng->ptrtonextmeaning;
	}
	return NULL;
}


/*
 * Function used to add all meaning of a word into a common string to be sent to client
 * Input is taken as address of word
 * Output is String containinig all meaning of a word seperated by delimiter(,)
 */
char *appendmeanings(struct w  *ptrtoword)
{
	int len = 0;
	struct m *temp1 = ptrtoword->ptrtomeaning;
	while(temp1)
	{
		len += strlen(temp1->meaning)+1;
		temp1=temp1->ptrtonextmeaning;
	}
	char *allmeanings = calloc( len ,sizeof(char) );
	struct m *temp = ptrtoword->ptrtomeaning;
	while(temp)
	{
		strcat(allmeanings,temp->meaning);
		if(temp->ptrtonextmeaning != NULL)
		{
			strcat(allmeanings,",");
		}
		temp = temp->ptrtonextmeaning;
	}

	return allmeanings;
}

/*
 * Function used as server side debugging
 * It prints all meanings of a particlar word
 */

void printallmeaningsforaword(struct m *ptr)
{
	int i=1;
	while(ptr)
	{
		printf("%d). %s\n",i,ptr->meaning);
		ptr=ptr->ptrtonextmeaning;
		i++;
	}
}

/*
 * Function used as server side debugging
 * It prints all meanings of  all words
 */
void print()
{
	struct w *temp = head;
	while(temp)
		 {
			 //printf("word is %s and meaning is %s\n",temp->word,temp->ptrtomeaning->meaning);
			printf("word is %s \n",temp->word);
			printallmeaningsforaword(temp->ptrtomeaning);
			temp=temp->ptrtonextword;
		 }
}


/*
 * Function used to create memory for new word to be added
 * Input is taken as word to be added
 * Ouput is Address to the word
 */
struct w* creatememoryforword(char *word)
{
	struct w *newword = (struct w*)malloc(sizeof(struct w));
	newword->word = (char *)malloc(strlen(word)*sizeof(char));
	strcpy(newword->word,word);
	newword->ptrtonextword = NULL;
	return newword;
}


/*
 * Function used to create memory for new meaning to be added
 * Input is taken as meaning to be added
 * Ouput is Address to the meaning
 */
struct m* creatememoryformeaning(char *meaning)
{
	struct m *newmeaning = (struct m*)malloc(sizeof(struct m));
	newmeaning->meaning = (char *)malloc(strlen(meaning)*sizeof(char));
	strcpy(newmeaning->meaning,meaning);
	newmeaning->ptrtonextmeaning = NULL;
	return newmeaning;
}


/*
 * Function used to insert a word with meaning
 * Input is input structure from client
 * return value is boolean which tell whether
 */
 bool_t insert_proc_1_svc(input *in,output *out,struct svc_req *rqstp)
{

	 bool_t retval = 1;;
	 if(head == NULL)
	 	{

		 	 head = creatememoryforword(in->word);
		 	 head->ptrtomeaning = creatememoryformeaning(in->meaning);

	 		 out->status = 1;
	 		 print();
	 		 return retval;
	 	}


	 struct w *indexofword = (struct w*)findindexofword(in->word);
	 if(indexofword == NULL)
	 {
		 printf("Word doesn't exists\n");
		 struct w *temp = head;
		 while(temp->ptrtonextword)
			 temp=temp->ptrtonextword;
		 struct w *newword = creatememoryforword(in->word);

		 	 newword->ptrtomeaning = creatememoryformeaning(in->meaning);

			temp->ptrtonextword = newword;
			out->status = 1;
			print();


	 }
	 else
	 {
		 printf("Word exists\n");
		 struct m *temp = indexofword->ptrtomeaning;
		 struct m *ismeaningpresent = findindexofmeaninginaword(indexofword,in->meaning);
		 if(ismeaningpresent != NULL)
		 {
			 out->status = 2;
			 return retval;
		 }
		 while(temp->ptrtonextmeaning)
		 {
			 if(strcmp(temp->meaning,in->meaning) >= 0)
			 {
				 break;
			 }
			temp=temp->ptrtonextmeaning;
		 }

		 if(temp->ptrtonextmeaning == NULL)
		 {
			 struct m *newmeaning = creatememoryformeaning(in->meaning);
			 		out->status = 1;
			 		temp->ptrtonextmeaning = newmeaning;
		 }
		 else
		 {
			 struct m *newmeaning = creatememoryformeaning(in->meaning);
			 struct m *temporary;
			 newmeaning->ptrtonextmeaning = temp->ptrtonextmeaning;
			 temp->ptrtonextmeaning = newmeaning;
		 }


		print();


	 }


	 out->status = 1;

	return retval;
}

 bool_t getmeaning_proc_1_svc(input *in,output *out,struct svc_req *rqstp)
 {
	 struct w *indexofword = findindexofword(in->word);
	 if(indexofword == NULL)
	 		{
	 			out->status = 0;
	 			printf("Word Not Present in dictionary!!\n");
	 		}
	 		else
	 		{
	 			printf("Key exists \n");
	 			out->status = 1;

	 			strcpy(out->word ,indexofword->word);
	 			char *allmeanings = appendmeanings(indexofword);
	 			printf("%s\n",allmeanings);
	 			strcpy(out->allmeaningsappended ,allmeanings);
	 			free(allmeanings);

	 		}
	 return 1;
 }

 void deleteallmeaningsforspecificword(struct w *ptrtoword)
 {
	 struct m *temp = ptrtoword->ptrtomeaning;
	 struct m *todelete;
	 while(temp)
	 {
		 todelete = temp;
		 temp=temp->ptrtonextmeaning;
		 printf("meaning %s deleted\n",todelete->meaning);
		 free(todelete);
	 }
 }


 bool_t delete_proc_1_svc(input *in,output *out,struct svc_req *rqstp)
  {
	 struct w *indexofword = (struct w*)findindexofword(in->word);
	 if(indexofword == NULL)
	 {
		 out->status = 0;
		 return 1;
	 }
	 if(indexofword == head)
	 {
		 	 	 if(head->ptrtonextword == NULL)
		         {
		             printf("There is only one node. The list can't be made empty ");
		             deleteallmeaningsforspecificword(indexofword);
		             free(indexofword);
		             head = NULL;
		             out->status = 1;
		             return 1;
		         }
		 	 	 out->status = 1;
		 	 	 head = head->ptrtonextword;
		 	 	 deleteallmeaningsforspecificword(indexofword);
		 	 	 free(indexofword);

		         return 1;

	 }
	 // find the previous node
	     struct w *prev = head;
	     while(prev->ptrtonextword != NULL && prev->ptrtonextword != indexofword)
	         prev = prev->ptrtonextword;

	     // Check if node really exists in Linked List
	     if(prev->ptrtonextword == NULL)
	     {
	         printf("\n Given node is not present in Linked List");
	         out->status = 0;
	         		 return 1;
	     }

	     // Remove node from Linked List
	     prev->ptrtonextword = prev->ptrtonextword->ptrtonextword;
	     deleteallmeaningsforspecificword(indexofword);
	     // Free memory
	     out->status = 1;
	 printf("word %s deleted\n",indexofword->word);
	 free(indexofword);
 	 return 1;
  }




 int dictionary_prog_1_freeresult (SVCXPRT *svcprt, xdrproc_t xdr, caddr_t cadr)
 {
	 (void) xdr_free(xdr, cadr);
	 return 1;
 }
