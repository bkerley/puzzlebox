#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h>

#include "llist.h"

#define MAX_METHOD_LEN 4
#define MAX_PATH_LEN 64
#define MAX_DATE_LEN 64

#define HEADER_CHUNK_LEN 256

char* read_method();
char* read_path();

void consume_headers();

void send_response();

void date_header();

llist* headers;

int main() {
  headers = llist_create();
  char* method = read_method();
  char* path = read_path();

  //  consume_headers();

  send_response(method, path);

  return 0;
}

char* read_method() {
  char* method = calloc(MAX_METHOD_LEN + 1, sizeof(char));

  fgets(method, MAX_METHOD_LEN, stdin);

  // only support three-long methods (i.e. GET)
  if (0 == strcmp("GET", method)) return method;

  puts("HTTP/1.1 405 Method Not Allowed");
  date_header();
  puts("Content-type: text/plain");
  puts("");
  puts("Method not recognized, dang");
  puts(method);
  printf("%x%x%x%x\n", method[0], method[1], method[2], method[3]);
  puts("sorry :(");

  exit(-1);
}

char* read_path() {
  char* path = calloc(MAX_PATH_LEN + 1, sizeof(char));

  fgets(path, MAX_PATH_LEN, stdin);

  if (path[MAX_PATH_LEN - 1] == 0) return path;

  puts("HTTP/1.1 404 Not Found");
  date_header();
  puts("Content-type: text/plain");
  puts("");
  puts("Path is too long, sorry");
  puts(path);

  exit(-1);
}

void consume_headers() {
  char last_header = 0;
  char* header_buf = calloc(HEADER_CHUNK_LEN + 1, sizeof(char));
  do {
    fgets(header_buf, HEADER_CHUNK_LEN, stdin);
    if (strlen(header_buf) <= 1) last_header = 1;
  } while (last_header == 0);
}

void send_response(char* method, char* path) {
  puts("HTTP/1.1 200 OK");
  date_header();
  puts("Content-type: text/plain");
  puts("");
  puts("hey, that worked :D");
  puts(method);
  puts(path);
  puts("");
}

void date_header() {
  char* timestamp = calloc(MAX_DATE_LEN + 1, sizeof(char));
  time_t now = time(NULL);
  strftime(timestamp, MAX_DATE_LEN, "%a, %d %b %Y %T %Z",
           gmtime(&now));
  printf("Date: %s", timestamp);
}
