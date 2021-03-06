struct common_in
{
	char word[20];
	char meaning[100];
};

struct insert_out
{
	int iswordinserted;
};

struct getmeaning_out
{
	char word[20];
	char meaning[500]; 
	int noofmeanings;
	int iswordpresent;
};

struct delete_out
{
	int isworddeleted; 
};

program DICTIONARY_PROG {
	version DICTIONARY_VERS {
		insert_out INSERT_PROC(common_in) = 1;
		getmeaning_out GETMEANING_PROC(common_in) = 2;
		delete_out DELETE_PROC(common_in) = 3;
	} = 1;
} = 0x13451111;


