gcc -lnsl -o client client.c dictionary_clnt.c dictionary_xdr.c 
gcc -lrpcsvc -lnsl -o server server.c dictionary_svc.c dictionary_xdr.c
