/*
 * This is sample code generated by rpcgen.
 * These are only templates and you can use them
 * as a guideline for developing your own functions.
 */

#include "dictionary.h"


void
dictionary_prog_1(char *host)
{
	CLIENT *clnt;
	enum clnt_stat retval_1;
	output result_1;
	input  insert_proc_1_arg;
	enum clnt_stat retval_2;
	output result_2;
	input  getmeaning_proc_1_arg;
	enum clnt_stat retval_3;
	output result_3;
	input  delete_proc_1_arg;

#ifndef	DEBUG
	clnt = clnt_create (host, DICTIONARY_PROG, DICTIONARY_VERS, "udp");
	if (clnt == NULL) {
		clnt_pcreateerror (host);
		exit (1);
	}
#endif	/* DEBUG */

	retval_1 = insert_proc_1(&insert_proc_1_arg, &result_1, clnt);
	if (retval_1 != RPC_SUCCESS) {
		clnt_perror (clnt, "call failed");
	}
	retval_2 = getmeaning_proc_1(&getmeaning_proc_1_arg, &result_2, clnt);
	if (retval_2 != RPC_SUCCESS) {
		clnt_perror (clnt, "call failed");
	}
	retval_3 = delete_proc_1(&delete_proc_1_arg, &result_3, clnt);
	if (retval_3 != RPC_SUCCESS) {
		clnt_perror (clnt, "call failed");
	}
#ifndef	DEBUG
	clnt_destroy (clnt);
#endif	 /* DEBUG */
}


int
main (int argc, char *argv[])
{
	char *host;

	if (argc < 2) {
		printf ("usage: %s server_host\n", argv[0]);
		exit (1);
	}
	host = argv[1];
	dictionary_prog_1 (host);
exit (0);
}
