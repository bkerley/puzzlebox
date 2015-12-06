#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h>

#include "http.h"

char* http_read_method() {
  char* method = calloc(HTTP_MAX_METHOD_LEN + 1, sizeof(char));

  fgets(method, HTTP_MAX_METHOD_LEN, stdin);

  // only support three-long methods (i.e. GET)
  if (0 == strcmp("GET", method)) return method;

  puts("HTTP/1.1 405 Method Not Allowed");
  http_send_date_header();
  puts("Content-type: text/plain");
  puts("");
  puts("Method not recognized, dang");
  puts(method);
  printf("%x%x%x%x\n", method[0], method[1], method[2], method[3]);
  puts("sorry :(");

  exit(-1);
}

char* http_read_path() {
  char* path = calloc(HTTP_MAX_PATH_LEN + 1, sizeof(char));

  fgets(path, HTTP_MAX_PATH_LEN, stdin);

  if (path[HTTP_MAX_PATH_LEN - 1] == 0) return path;

  puts("HTTP/1.1 404 Not Found");
  http_send_date_header();
  puts("Content-type: text/plain");
  puts("");
  puts("Path is too long, sorry");
  puts(path);

  exit(-1);
}


void http_consume_headers() {
  char last_header = 0;
  char* header_buf = calloc(HTTP_HEADER_CHUNK_LEN + 1, sizeof(char));
  do {
    fgets(header_buf, HTTP_HEADER_CHUNK_LEN, stdin);
    if (strlen(header_buf) <= 1) last_header = 1;
  } while (last_header == 0);
}

void http_send_date_header() {
  char* timestamp = calloc(HTTP_MAX_DATE_LEN + 1, sizeof(char));
  time_t now = time(NULL);
  strftime(timestamp, HTTP_MAX_DATE_LEN, "%a, %d %b %Y %T %Z",
           gmtime(&now));
  printf("Date: %s\r\n", timestamp);
}

void http_puts(char* line) {
  printf("%s\r\n", line);
}
