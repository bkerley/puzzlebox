#include <stdlib.h>
#include <string.h>

#include "proplist.h"

proplist* proplist_create() {
  proplist* pl = calloc(sizeof(proplist), 1);
  pl->list = llist_create();

  return pl;
};

void proplist_insert(proplist* pl, char* key, char* value) {
  proplist_kv* pair = calloc(sizeof(proplist_kv), 1);
  pair->key = key;
  pair->value = value;
  llist_append(pl->list, (void*)pair);
}

char* proplist_get(proplist* pl, char* key) {
  __block char* found = NULL;

  llist_each(pl->list, ^(void* datum) {
      proplist_kv* pair = (proplist_kv*)datum;
      if (0 != strcmp(pair->key, key)) return;

      found = pair->value;
    });

  return found;
}

void proplist_each(proplist* pl, void (^block)(char* key, char* value)) {
  llist_each(pl->list, ^(void* datum) {
      proplist_kv* pair = (proplist_kv*) datum;
      block(pair->key, pair->value);
    });
}
