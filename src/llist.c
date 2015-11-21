#include <stdlib.h>
#include "llist.h"

llist* llist_create() {
  return (llist*)calloc(1, sizeof(llist));
}

void llist_destroy(llist* list) {
  llist_node* current = list->first;
  llist_node* next;

  while (current != NULL) {
    next = current->next;
    free(current);
    current = next;
  }

  free(list);
}

void llist_append(llist* list, void* datum) {
  llist_node* old_last = list->last;
  llist_node* new_last = calloc(1, sizeof(llist_node));

  new_last->datum = datum;
  old_last->next = new_last;
  list->last = new_last;
}

void llist_prepend(llist* list, void* datum) {
  llist_node* old_first = list->first;
  llist_node* new_first = calloc(1, sizeof(llist_node));

  new_first->datum = datum;
  new_first->next = old_first;
  list->first = new_first;
}

void llist_each(llist* list, void (^block)(void* datum)) {
  llist_node* current = list->first;

  while (current != NULL) {
    block(current->datum);
    current = current->next;
  }
}

llist_node* llist_nth_node(llist_node* currentt, size_t count);

void* llist_nth(llist* list, size_t count) {
  llist_node* found = llist_nth_node(list->first, count);
  return found->datum;
}

llist_node* llist_nth_node(llist_node* current, size_t count) {
  if (count == 0) return current;
  llist_node* next = current->next;
  if (next == NULL) return NULL;

  return llist_nth_node(next, count - 1);
}
