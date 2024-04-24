#include <rpc/rpc.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SERVER_ADDRESS "localhost"
#define PROGRAM_NUM TRANSFER_PROG 
#define VERSION_NUM TRANSFER_VERS   

int transfer_file(const char* filename, CLIENT *clnt);

int main() {
  char filename[BUFFER_SIZE];
  CLIENT *clnt;
  int result;

  printf("Enter the filename to send: ");
  fgets(filename, BUFFER_SIZE, stdin);
  filename[strcspn(filename, "\n")] = 0; 

  clnt = clnt_create(SERVER_ADDRESS, PROGRAM_NUM, VERSION_NUM, "udp");
  if (clnt == NULL) {
    clnt_pcreateerror(SERVER_ADDRESS);
    exit(1);
  }
  result = transfer_file(filename, clnt);

  if (result == 0) {
    printf("File sent successfully!\n");
  } else {
    printf("Error transferring file: %d\n", result);
  }
  clnt_destroy(clnt); 
  return 0;
}

int transfer_file(const char* filename, CLIENT *clnt) {
  int return_value;
  struct transfer_args args;
  strcpy(args.filename, filename);  

  XDR xdr_stream;
  xdrmem_create(&xdr_stream, (caddr_t) &args, sizeof(args), XDR_ENCODE);
  return_value = clnt_call(clnt, TRANSFER_PROC_1, (xdrproc_t) xdr_transfer_args, &args, (xdrproc_t) xdr_int, &return_value, svc_createerr);
  xdr_destroy(&xdr_stream);

  if (return_value != RPC_SUCCESS) {
    clnt_perror(clnt, "call failed");
  }
  return return_value; 
}

