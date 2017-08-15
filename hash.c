#include <assert.h>
#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TABLE_SIZE 50000
#define WORD_SIZE 100
const char TYPOS[27][9] = {
    "qwszx",     // a
    "ghvn",      // b
    "dfxv",      // c
    "wersfxcv",  // d
    "wrsdf",     // e
    "ertdgcvb",  // f
    "rtyfhvbn",  // g
    "tyugjbnm",  // h
    "uojkl",     // i
    "yuihknm",   // j
    "uiojlm",    // k
    "iopkç",     // l
    "jkn",       // m
    "hjbm",      // n
    "ipklç",     // o
    "olç",       // p
    "wsa",       // q
    "etdfg",     // r
    "qweadzxc",  // s
    "ryfgh",     // t
    "yihjk",     // u
    "fgcb",      // v
    "qeasd",     // w
    "asdzc",     // x
    "tughj",     // y
    "asx",       // z
    "opl",       // ç
};

struct node {
  char *key;    // word (i.e: hosue)
  char *value;  // reference word (i.e: house)
  struct node *next;
};

struct linked_list {
  struct node *head;
  int length;  // Current number of nodes
};

struct hashtable {
  struct linked_list **hashtable;
  unsigned int table_size;
};

unsigned long hash(char *str);
struct node *node__create(char *key, char *value);
int node__destroy(struct node *target);
struct linked_list *linked_list__init();
struct linked_list *linked_list__init_with_keyval(char *key, char *value);
int linked_list__insert(struct linked_list *list, char *key, char *value);
struct node *linked_list__delete(struct linked_list *list, char *key);
struct node *linked_list__find(struct linked_list *list, char *key);
void *linked_list__destroy(struct linked_list *list);
struct hashtable *hashtable__init(int table_size);
int hashtable__destroy(struct hashtable *hashtable);
struct hashtable *hashtable__insert(struct hashtable *hashtable, char *key,
                                    char *value);
struct node *hashtable__find(struct hashtable *hashtable, char *key);
struct node *hashtable__delete(struct hashtable *hashtable, char *key);
struct hashtable *spellchecker__insert(struct hashtable *hashtable, char *key);
bool spellchecker__delete(struct hashtable *hashtable, char *key);
void swap_chars(char *chars, int first, int second);

int main(int argc, char **argv) {
  struct hashtable *hashtable = hashtable__init(TABLE_SIZE);

  assert(hashtable != NULL);

  char *command = calloc(WORD_SIZE, sizeof(char));
  char *last_word = calloc(WORD_SIZE, sizeof(char));
  struct node *query_result = NULL;
  bool has_last_word = false;

  while (scanf(" %[^\n]s", command) == 1) {
    switch (command[0]) {
      case '+':
        query_result = hashtable__find(hashtable, last_word);

        if (query_result == NULL) {
          if (!has_last_word) {
            printf("fail\n");
            break;
          }
          spellchecker__insert(hashtable, last_word);
          assert(hashtable != NULL);
          printf("ok\n");
        } else {
          printf("fail\n");
        }
        break;
      case '-':
        if (spellchecker__delete(hashtable, last_word)) {
          printf("ok\n");
        } else {
          printf("fail\n");
        }
        break;
      case '*':  // Free everything, end program
        hashtable__destroy(hashtable);
        free(hashtable);
        free(command);
        free(last_word);
        return 0;
      default:  // check word exiseance
        assert(isalpha(command[0]));

        query_result = hashtable__find(hashtable, command);

        if (query_result != NULL)
          printf("%s\n", query_result->value);
        else
          printf("not found\n");

        strcpy(last_word, command);
        has_last_word = true;

        break;
    }
  }

  // Free everything
  hashtable__destroy(hashtable);
  free(hashtable);
  free(command);
  // free(last_word);

  return 0;
}

unsigned long hash(char *str) {
  unsigned long hash = 5381;
  int c;

  while ((c = *str++)) hash = ((hash << 5) + hash) + c;

  return hash % TABLE_SIZE;
}

struct node *node__create(char *key, char *value) {
  assert(key != NULL);
  assert(value != NULL);

  struct node *newItem = malloc(sizeof(struct node));

  assert(newItem != NULL);

  newItem->key = calloc(strlen(key) + 1, sizeof(char));
  newItem->value = calloc(strlen(value) + 1, sizeof(char));
  newItem->next = NULL;

  strcpy(newItem->key, key);
  strcpy(newItem->value, value);

  return newItem;
}

int node__destroy(struct node *target) {
  if (target == NULL) return 0;
  free(target->key);
  free(target->value);
  free(target);
  return 0;
}

int linked_list__insert(struct linked_list *list, char *key, char *value) {
  assert(list != NULL);
  assert(key != NULL);
  assert(value != NULL);

  struct node **target = &list->head;

  while (*target != NULL) {
    if (strcmp((*target)->key, key) == 0) { // If an equal key already is on the list
      return 0;
    }
    target = &(*target)->next;
  }

  *target = node__create(key, value);

  assert (*target != NULL);

  list->length++;

  return 0;
}

struct node *linked_list__delete(struct linked_list *list, char *key) {
  if (list == NULL || key == NULL) return NULL;

  struct node **target = &(list->head);

  while (*target && strcmp((*target)->key, key) != 0) target = &(*target)->next;

  if (*target == NULL) return NULL;

  struct node *deletedNode = *target;
  *target = deletedNode->next;
  deletedNode->next = NULL;

  return deletedNode;
}

struct node *linked_list__find(struct linked_list *list, char *key) {
  assert(list != NULL);
  assert(key != NULL);

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

    while (next != NULL) {
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
  struct linked_list *newList = malloc(sizeof(struct linked_list));

  assert(newList != NULL);

  newList->length = 0;
  newList->head = NULL;
  return newList;
}

struct linked_list *linked_list__init_with_keyval(char *key, char *value) {
  struct linked_list *newList = linked_list__init();

  struct node *firstNode = node__create(key, value);
  newList->head = firstNode;

  return newList;
}

struct hashtable *hashtable__init(int table_size) {
  struct hashtable *new_hashtable = malloc(sizeof(struct hashtable));

  if (new_hashtable == NULL) {
    return NULL;
  }

  new_hashtable->hashtable = calloc(TABLE_SIZE, sizeof(struct linked_list *));

  new_hashtable->table_size = table_size;

  return new_hashtable;
}

int hashtable__destroy(struct hashtable *hashtable) {
  assert(hashtable != NULL);
  
  for (int i = 0; i < hashtable->table_size; i++) {
    linked_list__destroy(hashtable->hashtable[i]);
  }

  free(hashtable->hashtable);

  return 0;
}

struct hashtable *hashtable__insert(struct hashtable *hashtable, char *key,
                                    char *value) {
  unsigned long index = hash(key);

  if (hashtable->hashtable[index] == NULL) {
    hashtable->hashtable[index] = linked_list__init();
  }

  assert(hashtable->hashtable[index] != NULL);

  linked_list__insert(hashtable->hashtable[index], key, value);

  return hashtable;
}

struct node *hashtable__find(struct hashtable *hashtable, char *key) {
  assert(hashtable != NULL);
  assert(hashtable->hashtable != NULL);
  assert(key != NULL);

  unsigned long index = hash(key);

  if (hashtable->hashtable[index] == NULL) {
    return NULL;
  }

  return linked_list__find(hashtable->hashtable[index], key);
}

struct node *hashtable__delete(struct hashtable *hashtable, char *key) {
  assert(hashtable != NULL);
  assert(key != NULL);

  unsigned long index = hash(key);
  return linked_list__delete(hashtable->hashtable[index], key);
}

struct hashtable *spellchecker__insert(struct hashtable *hashtable, char *key) {
  assert(hashtable != NULL);
  assert(key != NULL);
  
  int keyLength = strlen(key);
  char *originalKey = calloc(keyLength + 1, sizeof(char));

  assert(originalKey != NULL);

  originalKey = strcpy(originalKey, key);

  hashtable__insert(hashtable, key, key);

  for (int i = 0; i < keyLength - 1; i++) {  // Swaped chars
    swap_chars(key, i, i + 1);
    hashtable__insert(hashtable, key, originalKey);
    swap_chars(key, i, i + 1);
  }

  for (int i = 0; i < keyLength; i++) {  // Duplicate char
    memmove(&key[i + 1], &key[i], keyLength - i * sizeof(char));
    key[i] = key[i + 1];
    hashtable__insert(hashtable, key, originalKey);
    strcpy(key, originalKey);
  }

  for (int i = 0; i < keyLength; i++) {  // Char missing
    memmove(&key[i], &key[i + 1], keyLength - i * sizeof(char));
    hashtable__insert(hashtable, key, originalKey);
    strcpy(key, originalKey);
  }

  char originalChar;
  for (int i = 0; i < keyLength; i++) {  // Typos
    originalChar = key[i];

    char c = TYPOS[originalChar - 'a'][0];
    for (int j = 1; c != '\0'; j++) {
      key[i] = c;
      hashtable__insert(hashtable, key, originalKey);
      c = (TYPOS[originalChar - 'a'])[j];
    }

    key[i] = originalChar;
  }

  free(originalKey);

  return NULL;
}

bool spellchecker__delete(struct hashtable *hashtable, char *key) {
  assert(hashtable != NULL);
  assert(key != NULL);

  struct node *query_result = hashtable__delete(hashtable, key);
  if (query_result == NULL) {
    return false;
  } else {
    node__destroy(query_result);
  }
  
  int keyLength = strlen(key);
  char *originalKey = calloc(keyLength + 1, sizeof(char));

  assert(originalKey != NULL);

  originalKey = strcpy(originalKey, key);

  for (int i = 0; i < keyLength - 1; i++) {  // Swaped chars
    swap_chars(key, i, i + 1);
    node__destroy(hashtable__delete(hashtable, key));
    swap_chars(key, i, i + 1);
  }

  for (int i = 0; i < keyLength; i++) {  // Duplicate char
    memmove(&key[i + 1], &key[i], keyLength - i * sizeof(char));
    key[i] = key[i + 1];
    node__destroy(hashtable__delete(hashtable, key));
    strcpy(key, originalKey); // TODO: try to just move it back
  }

  for (int i = 0; i < keyLength; i++) {  // Char missing
    memmove(&key[i], &key[i + 1], keyLength - i * sizeof(char));
    node__destroy(hashtable__delete(hashtable, key));
    strcpy(key, originalKey);
  }

  char originalChar;
  for (int i = 0; i < keyLength; i++) {  // Typos
    originalChar = key[i];

    char c = TYPOS[originalChar - 'a'][0];
    for (int j = 1; c != '\0'; j++) {
      key[i] = c;
      node__destroy(hashtable__delete(hashtable, key));
      c = (TYPOS[originalChar - 'a'])[j];
    }

    key[i] = originalChar;
  }

  free(originalKey);

  return true;
}

void swap_chars(char *chars, int first, int second) {
  if (chars == NULL || first < 0 || second < 0) return;

  char current = chars[first];
  chars[first] = chars[second];
  chars[second] = current;

  return;
}
