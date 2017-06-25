#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define TABLE_SIZE 100000
#define WORD_SIZE 100


unsigned long hash(unsigned char *str) {
  unsigned long hash = 5381;
  int c;

  while (c = *str++)
    hash = ((hash << 5) + hash) + c; /* hash * 33 + c */

  return hash % TABLE_SIZE;
}

int main(int argc, char **argv) {
  char string[WORD_SIZE];
  char ** hashtable;
  hashtable = calloc(TABLE_SIZE, sizeof(char **));
  for (int i = 0; i < TABLE_SIZE; i++) {
    hashtable[i] = calloc(WORD_SIZE, sizeof(char *));
    memset(hashtable[i], '\0', sizeof(char *) * WORD_SIZE);
  }

  while (scanf(" %[^\n]s", string) == 1) {
    unsigned long index = hash(string);
    strcpy(hashtable[index], string);
  }

  return 0;
}