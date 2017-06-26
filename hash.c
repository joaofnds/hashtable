#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define TABLE_SIZE 100
#define WORD_SIZE 100

struct node {
  char * key;
  int value;
  struct node *next;
};

struct linked_list {
  struct node * head;
  int length; // Current number of nodes
};

unsigned long hash(unsigned char *str);
struct node        *node__create(char *key, int value);
int                 node__destroy(struct node *target);
struct linked_list *linked_list__init();
struct linked_list *linked_list__init_with_keyval(char *key, int value);
int                 linked_list__insert(struct linked_list *list, char *key, int value);
struct node        *linked_list__delete(struct linked_list *list, char *key);
struct node        *linked_list__find(struct linked_list *list, char *key);
void               *linked_list__destroy(struct linked_list *list);


int main(int argc, char **argv) {
  struct linked_list **hashtable;
  hashtable = (struct linked_list **) calloc(TABLE_SIZE, sizeof(struct linked_list *));

  int value = 0;
  char key[WORD_SIZE];
  while (scanf(" %[^\n]s", key) == 1) {
    unsigned long index = hash(key);

    if (hashtable[index] == NULL) {
      hashtable[index] = linked_list__init();
    }

    if (hashtable[index] == NULL) {
      printf("FAILED TO CREATE LINKED LIST AT INDEX \"%d\"\n", value);
      continue;
    }

    linked_list__insert(hashtable[index], key, value++);
  }

  for (int i = 0; i < TABLE_SIZE; i++) {
    hashtable[i] = linked_list__destroy(hashtable[i]);
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
  if (key == NULL) return NULL;

  struct node *newItem = (struct node *) malloc(sizeof(struct node));
  newItem->key = (char *) calloc(strlen(key) + 1, sizeof(char));
  newItem->value = value;
  newItem->next = NULL;

  strcpy(newItem->key, key);

  return newItem;
}

int node__destroy(struct node *target) {
  if (target == NULL)
    return 0;
  free(target->key);
  free(target);
  return 0;
}

int linked_list__insert(struct linked_list *list, char *key, int value) {
  if (list == NULL || key == NULL) return -1;

  struct node **target = &list->head;

  while (*target != NULL) {
    if (strcmp((*target)->key, key) == 0) {
      printf("[linked_list__init] FOUND EQUAL KEY: \"%s\"\n", key);
      return 0;
    }
    target = &(*target)->next;
  }

  *target = node__create(key, value);

  if (*target == NULL) {
    printf("[linked_list__insert] FAILED TO CREATE NODE {\"%s\": \"%d\"\n", key, value);
    return -1;
  }

  list->length++;

  return 0;
}

struct node * linked_list__delete(struct linked_list *list, char *key) {
  // TODO: implement functionality
  return 0;
}

struct node * linked_list__find(struct linked_list *list, char *key) {
  if (list == NULL || key == NULL) return NULL;

  struct node **target = &list->head;

  while (*target != NULL) {
    if (strcmp((*target)->key, key) == 0) return *target;
    target = &(*target)->next;
  }

  return NULL;
}

void *linked_list__destroy(struct linked_list *list) {
  if (list == NULL) return NULL;
  
  struct node *target = list->head;
  if (target != NULL) {
    struct node *next = target->next;

    while(next != NULL) {
      node__destroy(target);
      list->length--;
      target = next;
      next = target->next;
    }
    node__destroy(target);
    target = NULL;
  }

  free(list);

  return NULL;
}

struct linked_list *linked_list__init() {
  struct linked_list *newList = (struct linked_list *) malloc(sizeof(struct linked_list));

  if (newList == NULL) return NULL;

  newList->length = 0;
  newList->head = NULL;
  return newList;
}

struct linked_list *linked_list__init_with_keyval(char *key, int value) {
  struct linked_list *newList = linked_list__init();

  struct node *firstNode = node__create(key, value);
  newList->head = firstNode;

  return newList;
}
