#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>  
#include <sys/stat.h>  

#define BUFFER_SIZE 1024

int main(int argc, char *argv[]) {
  MPI_Init(&argc, &argv);

  int rank, size;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);

  if (rank != 0) {
    MPI_Finalize();
    return 0;
  }

  char filename[BUFFER_SIZE];
  int file_fd;
  struct stat file_stat;

  while (1) {
    MPI_Recv(filename, BUFFER_SIZE, MPI_CHAR, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    if (access(filename, R_OK) != 0) {
      printf("Server: Access denied for '%s'\n", filename);
      continue;
    }

    if (stat(filename, &file_stat) < 0 || !S_ISREG(file_stat.st_mode)) {
      printf("Server: '%s' is not a regular file.\n", filename);
      continue;
    }

    file_fd = open(filename, O_RDONLY);
    if (file_fd < 0) {
      printf("Server: Error opening file '%s'\n", filename);
      continue;
    }

    int file_size = file_stat.st_size;
    MPI_Send(&file_size, 1, MPI_INT, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD);

    int bytes_read;
    char buffer[BUFFER_SIZE];
    while ((bytes_read = read(file_fd, buffer, BUFFER_SIZE)) > 0) {
      MPI_Send(buffer, bytes_read, MPI_CHAR, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD);
    }

    close(file_fd);
  }

  MPI_Finalize();
  return 0;
}
