#pragma once

typedef struct llist_node_s {
  void* datum;
  struct llist_node_s* next;
} llist_node;

typedef struct llist_s {
  struct llist_node_s* first;
  struct llist_node_s* last;
} llist;

llist* llist_create();
void llist_destroy(llist* list);
void llist_append(llist* list, void* datum);
void llist_prepend(llist* list, void* datum);
void llist_each(llist* list, void (^block)(void* datum));
void* llist_nth(llist* list, size_t count);
