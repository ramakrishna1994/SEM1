struct input
{
	char word[20];
	char meaning[100];
};

struct output
{
	char word[20];
	char allmeaningsappended[300];
	int status;
};

program DICTIONARY_PROG {
	version DICTIONARY_VERS {
		output INSERT_PROC(input) = 1;
		output GETMEANING_PROC(input) = 2;
		output DELETE_PROC(input) = 3;
	} = 1;
} = 0x13451111;


