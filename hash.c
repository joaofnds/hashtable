#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define TABLE_SIZE 10000
#define WORD_SIZE 100

struct item {
  char * key;
  int * value;
};

unsigned long hash(unsigned char *str) {
  unsigned long hash = 5381;
  int c;

  while (c = *str++)
    hash = ((hash << 5) + hash) + c;

  return hash % TABLE_SIZE;
}

struct item *createItem(char *key, int value) {
  struct item *newItem = (struct item *) malloc(sizeof(struct item *));
  newItem->key = calloc(WORD_SIZE, sizeof(char));
  newItem->value = malloc(sizeof(int));
  
  strcpy(newItem->key, key);
  *newItem->value = value;
  
  return newItem;
}

int main(int argc, char **argv) {
  struct item ** hashtable;
  hashtable = (struct item **) calloc(TABLE_SIZE, sizeof(struct item **));

  int keyId = 0;
  char key[WORD_SIZE];
  while (scanf(" %[^\n]s", key) == 1) {
    unsigned long index = hash(key);
    hashtable[index] = createItem(key, keyId++);
  }

  for (int i = 0; i < TABLE_SIZE; i++) {
    if (hashtable[i]->key && hashtable[i]->value)
      printf("'%s': '%d'\n", hashtable[i]->key, *hashtable[i]->value);
  }

  return 0;
}