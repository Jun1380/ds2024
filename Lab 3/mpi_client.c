#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFFER_SIZE 1024

int main(int argc, char *argv[]) {
  MPI_Init(&argc, &argv);

  int rank, size;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);

  if (rank == 0) {
    printf("Client (rank %d): Enter the filename to transfer: ", rank);
    char filename[BUFFER_SIZE];
    fgets(filename, BUFFER_SIZE, stdin);
    filename[strcspn(filename, "\n")] = 0; 

    MPI_Send(filename, BUFFER_SIZE, MPI_CHAR, 0, 0, MPI_COMM_WORLD);

    int file_size;
    MPI_Recv(&file_size, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    printf("Server: File size: %d bytes\n", file_size); 

    int bytes_received;
    char buffer[BUFFER_SIZE];
    FILE *output_file = fopen("received_file.txt", "wb");  
    while ((bytes_received = MPI_Recv(buffer, BUFFER_SIZE, MPI_CHAR, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE)) > 0) {
      fwrite(buffer, sizeof(char), bytes_received, output_file);
    }

    fclose(output_file);
    printf("Client: File transfer complete.\n");
  }

  MPI_Finalize();
  return 0;
}
