#include <rpc/rpc.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

#define BUFFER_SIZE 1024
#define PROGRAM_NUM TRANSFER_PROG 
#define VERSION_NUM TRANSFER_VERS  

struct transfer_args {
  char filename[BUFFER_SIZE];
};

int transfer_file_1(struct transfer_args *args, int *result, struct svc_req *req);

int main(){
  SVCXPRT *transp;

  transp = svcudp_create(rpc_any_port);
  if (transp == NULL) {
    perror("svcudp_create");
    exit(1);
  }

  if (registerrpc(transp, PROGRAM_NUM, VERSION_NUM, transfer_file_1, NULL, NULL) != 0) {
    perror("registerrpc");
    exit(1);
  }

  svc_run(transp);
  svc_destroy(transp);

  return 0;
}

int transfer_file_1(struct transfer_args *args, int *result, struct svc_req *req) {
  int file_fd;
  int bytes_read;
  char buffer[BUFFER_SIZE];

  file_fd = open(args->filename, O_RDONLY);
  if (file_fd < 0) {
    perror("open");
    *result = -1; 
    return 1; /
  }

  while ((bytes_read = read(file_fd, buffer, BUFFER_SIZE)) > 0) {
  XDR xdr_stream;
  xdrmem_create(&xdr_stream, (caddr_t) &buffer, bytes_read, XDR_ENCODE);
  
  struct data_chunk {
    int data_size;
    char data[BUFFER_SIZE];
  };

  struct data_chunk chunk;
  chunk.data_size = bytes_read;
  memcpy(chunk.data, buffer, bytes_read);

  bool success = xdr_struct(&xdr_stream, (xdrproc_t) xdr_data_chunk, &chunk);
  if (!success) {
    perror("xdr_struct");
    close(file_fd);
    *result = -1;
    return 1; 
  }

  success = svc_sendreply(req, TRUE, (xdrproc_t) xdr_void);
  if (!success) {
    perror("svc_sendreply");
    close(file_fd);
    *result = -1; 
    return 1; 
  }

  xdr_destroy(&xdr_stream);
}
  close(file_fd);
  *result = 0; 
  return
}