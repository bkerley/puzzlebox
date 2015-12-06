#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h>

#include "http.h"

void fsend(FILE* read);

char* http_read_method() {
  char* method = calloc(HTTP_MAX_METHOD_LEN + 1, sizeof(char));

  fgets(method, HTTP_MAX_METHOD_LEN, stdin);

  return method;

  exit(-1);
}

char* http_read_path() {
  char* path = calloc(HTTP_MAX_PATH_LEN + 1, sizeof(char));

  fgets(path, HTTP_MAX_PATH_LEN, stdin);

  if (path[HTTP_MAX_PATH_LEN - 1] == 0) return path;

  http_fail(404,
            "Not Found",
            "Path is too long, sorry");

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

void http_fail(int code, char* failmesg, char* longmesg) {
  printf("HTTP/1.1 %d %s\r\n", code, failmesg);
  http_send_date_header();
  http_puts("Content-type: text/html");
  http_puts("");

  FILE* preamble = fopen("priv/error_preamble.html", "r");
  FILE* postamble = fopen("priv/error_postamble.html", "r");

  fsend(preamble);
  puts(longmesg);
  fsend(postamble);
  fclose(preamble);
  fclose(postamble);
  http_puts("");
}

void http_puts(char* line) {
  printf("%s\r\n", line);
}

void fsend(FILE* in) {
  fseek(in, 0, SEEK_END);
  size_t count = ftello(in);
  rewind(in);

  char* buf = calloc(sizeof(char), count);

  fread(buf, sizeof(char), count, in);
  fwrite(buf, sizeof(char), count, stdout);
}
