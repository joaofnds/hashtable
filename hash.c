#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define TABLE_SIZE 10000
#define WORD_SIZE 100

struct node {
  char * key;
  int * value;
  struct node *next;
};

struct linked_list {
  struct node * head;
  int length; // Current number of nodes
};

unsigned long hash(unsigned char *str);
struct node        *node__create(char *key, int value);
struct linked_list *linked_list__init();
struct linked_list *linked_list__init_with_keyval(char *key, int value);
int                 linked_list__insert(struct linked_list *list, char *key, int value);
struct node        *linked_list__delete(struct linked_list *list, char *key);
int                 linked_list__find(struct linked_list *list, char *key);
int                 linked_list__destroy(struct linked_list *list);


int main(int argc, char **argv) {
  struct linked_list **hashtable;
  hashtable = (struct linked_list **) calloc(TABLE_SIZE, sizeof(struct linked_list **));

  int value = 0;
  char key[WORD_SIZE];
  while (scanf(" %[^\n]s", key) == 1) {
    unsigned long index = hash(key);

    if (hashtable[index] == NULL) {
      hashtable[index] = linked_list__init();
    }

    if (hashtable[index] == NULL) continue;

    linked_list__insert(hashtable[index], key, value++);
  }

  for (int i = 0; i < TABLE_SIZE; i++) {
    if (hashtable[i] != NULL) {
      linked_list__destroy(hashtable[i]);
    }
  }

  free(hashtable);

  return 0;
}


unsigned long hash(unsigned char *str) {
  unsigned long hash = 5381;
  int c;

  while (c = *str++)
    hash = ((hash << 5) + hash) + c;

  return hash % TABLE_SIZE;
}

struct node *node__create(char *key, int value) {
  struct node *newItem = (struct node *) malloc(sizeof(struct node *));
  newItem->key = calloc(WORD_SIZE, sizeof(char));
  newItem->value = malloc(sizeof(int));
  newItem->next = NULL;
  
  strcpy(newItem->key, key);
  *newItem->value = value;
  
  return newItem;
}

int linked_list__insert(struct linked_list *list, char *key, int value) {
  // TODO: implement functionality
  return 0;
}

struct node * linked_list__delete(struct linked_list *list, char *key) {
  // TODO: implement functionality
  return 0;
}

int linked_list__find(struct linked_list *list, char *key) {
  // TODO: implement functionality
  return 0;
}

int linked_list__destroy(struct linked_list *list) {
  struct node *target = list->head;
  if (target != NULL) {
    struct node *next = target->next;
    while(next != NULL) {
      free(target);
      target = next;
      next = target->next;
    }
  }

  free(target);
  free(list);

  return 1;
}

struct linked_list *linked_list__init() {
  struct linked_list *newList = (struct linked_list *) malloc(sizeof(struct linked_list*));

  if (newList == NULL) return NULL;

  newList->length = 0;
  return newList;
}

struct linked_list *linked_list__init_with_keyval(char *key, int value) {
  struct linked_list *newList = linked_list__init();

  struct node *firstNode = node__create(key, value);
  newList->head = firstNode;

  return newList;
}
