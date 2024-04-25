#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_WORD_LENGTH 100

typedef struct {
  char word[MAX_WORD_LENGTH];
  int count;
} WordCount;

void map_function(const char* line, WordCount** intermediate_result, int* size) {
  char word[MAX_WORD_LENGTH];
  FILE* stream = fmemopen((void*)line, strlen(line), "r");
  while (fscanf(stream, "%s", word) != EOF) {
    int found = 0;
    for (int i = 0; i < *size; i++) {
      if (strcmp((*intermediate_result)[i].word, word) == 0) {
        (*intermediate_result)[i].count++;
        found = 1;
        break;
      }
    }
    if (!found) {
      if (*size == 0) {
        *intermediate_result = malloc(sizeof(WordCount));
        if (*intermediate_result == NULL) {
          fprintf(stderr, "Memory allocation failed\n");
          exit(1);
        }
        *size = 1;
      } else {

        WordCount* temp = realloc(*intermediate_result, sizeof(WordCount) * (*size + 1));
        if (temp == NULL) {
          fprintf(stderr, "Memory allocation failed\n");
          exit(1);
        }
        *intermediate_result = temp;
        (*size)++;
      }
      strcpy((*intermediate_result)[*size - 1].word, word);
      (*intermediate_result)[*size - 1].count = 1;
    }
  }
  fclose(stream);
}

void reduce_function(const WordCount* intermediate_result, int size) {
  for (int i = 0; i < size; i++) {
    printf("%s\t%d\n", intermediate_result[i].word, intermediate_result[i].count);
  }
}

int main() {
  WordCount* intermediate_result = NULL;
  int result_size = 0;

  const char* input_data[] = {"Hello it's me Mario\n", "Mario is my favourite character\n", "But why I like Mario\n"};
  int num_lines = sizeof(input_data) / sizeof(input_data[0]);

  for (int i = 0; i < num_lines; i++) {
    map_function(input_data[i], &intermediate_result, &result_size);
  }

  reduce_function(intermediate_result, result_size);

  free(intermediate_result);

  return 0;
}
