#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h>

#include "llist.h"
#include "http.h"


void send_response();

llist* headers;

int main() {
  headers = llist_create();
  char* method = http_read_method();
  char* path = http_read_path();

  //  consume_headers();

  send_response(method, path);

  return 0;
}

void send_response(char* method, char* path) {
  http_puts("HTTP/1.1 200 OK");
  http_send_date_header();
  http_puts("Content-type: text/plain");
  http_puts("");
  http_puts("hey, welcome to the puzzle box");
  printf("method: %s\r\n", method);
  printf("path: %s\r\n", path);
  http_puts("");
}
