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

struct hashtable {
  struct linked_list **hashtable;
  unsigned int table_size;
};

unsigned long hash(char *str);
struct node        *node__create(char *key, int value);
int                 node__destroy(struct node *target);
struct linked_list *linked_list__init();
struct linked_list *linked_list__init_with_keyval(char *key, int value);
int                 linked_list__insert(struct linked_list *list, char *key, int value);
struct node        *linked_list__delete(struct linked_list *list, char *key);
struct node        *linked_list__find(struct linked_list *list, char *key);
void               *linked_list__destroy(struct linked_list *list);
struct hashtable   *hashtable__init(int table_size);
int                 hashtable__destroy(struct hashtable *hashtable);
struct hashtable   *hashtable__insert(struct hashtable *hashtable, char *key, int value);
struct node        *hashtable__find(struct hashtable *hashtable, char *key);
struct hashtable   *hashtable__delete(struct hashtable *hashtable, char *key);


int main(int argc, char **argv) {
  struct hashtable *hashtable = hashtable__init(TABLE_SIZE);

  if (hashtable == NULL) {
    printf("FAILED TO ALLOCATE MEMORY FOR THE HASHTABLE\n");
    return 0;
  }

  int value = 0;
  char key[WORD_SIZE];
  while (scanf(" %[^\n]s", key) == 1) {
    hashtable = hashtable__insert(hashtable, key, value);
    value += 1;
    // if (value == 500) break;
  }
  
  // while (scanf(" %[^\n]s", key) == 1) {
  //   struct node *find_result = hashtable__find(hashtable, key);
  //   if (find_result != NULL)
  //     printf("'%s': '%d'\n", find_result->key, find_result->value);
  // }

  // Free everything
  hashtable__destroy(hashtable);
  free(hashtable);

  return 0;
}


unsigned long hash(char *str) {
  unsigned long hash = 5381;
  int c;

  while ((c = *str++))
    hash = ((hash << 5) + hash) + c;

  return hash % TABLE_SIZE;
}

struct node *node__create(char *key, int value) {
  if (key == NULL) return NULL;

  struct node *newItem = (struct node *) malloc(sizeof(struct node));

  if (newItem == NULL) {
    printf("FAILED TO CREATE NEW NODE {%s: %d}\n", key, value);
  }
  
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
  if (list == NULL || key == NULL) return NULL;

  struct node *target = list->head;
  struct node *match;

  while (target != NULL) {
    if (target->next == NULL) {
      return NULL;
    }
    
    if (strcmp(target->next->key, key) == 0) { // If keys match
      match = target->next;
      target->next = match->next;
      return match;
    }

    target = target->next;
  }
  return NULL;
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

struct hashtable *hashtable__init(int table_size) {
  struct hashtable *new_hashtable =  (struct hashtable *) malloc(sizeof(struct hashtable));
  
  if (new_hashtable == NULL) {
    return NULL;
  }

  new_hashtable->hashtable = (struct linked_list **) calloc(TABLE_SIZE, sizeof(struct linked_list *));

  new_hashtable->table_size = table_size;

  return new_hashtable;  
}

int hashtable__destroy(struct hashtable *hashtable) {
  if (hashtable == NULL) return 0;

  for (int i = 0; i < hashtable->table_size; i++) {
    linked_list__destroy(hashtable->hashtable[i]);
  }

  free(hashtable->hashtable);

  return 0;
}

struct hashtable *hashtable__insert(struct hashtable *hashtable, char *key, int value) {
    unsigned long index = hash(key);

    if (hashtable->hashtable[index] == NULL) {
      hashtable->hashtable[index] = linked_list__init();
    }

    if (hashtable->hashtable[index] == NULL) {
      printf("FAILED TO CREATE LINKED LIST AT INDEX \"%d\"\n", value);
      return hashtable;
    }

    linked_list__insert(hashtable->hashtable[index], key, value++);

    return hashtable;
}

struct node *hashtable__find(struct hashtable *hashtable, char *key) {
  unsigned long index = hash(key);

  if (hashtable->hashtable[index] == NULL) {
    return NULL;
  }

  return linked_list__find(hashtable->hashtable[index], key);
}

struct hashtable *hashtable__delete(struct hashtable *hashtable, char *key) {
  unsigned long index = hash(key);
  linked_list__delete(hashtable->hashtable[index], key);
  return hashtable;
}

