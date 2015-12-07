#pragma once

#include "llist.h"

typedef struct proplist_kv_s {
  char* key;
  char* value;
} proplist_kv;

typedef struct proplist_s {
  llist* list;
} proplist;

proplist* proplist_create();
void proplist_insert(proplist* pl, char* key, char* value);
char* proplist_get(proplist* pl, char* key);
void proplist_each(proplist* pl, void (^block)(char* key, char* value));
