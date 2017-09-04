struct input
{
	char word[20];
};

struct output
{
	char mean[20];
};

program DICTIONARY_PROG {
	version DICTIONARY_VERS {
		output INSERT_PROC(input) = 1;
	} = 1;
} = 0x13451111;


