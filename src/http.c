#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h>

#include "http.h"
#include "proplist.h"

void fsend(FILE* read);
void read_to_space(char* buf, size_t max_len);
void consume_http_version();

char* http_read_method() {
  char* method = calloc(HTTP_MAX_METHOD_LEN + 1, sizeof(char));

  read_to_space(method, HTTP_MAX_METHOD_LEN);

  return method;

  exit(-1);
}

char* http_read_path() {
  char* path = calloc(HTTP_MAX_PATH_LEN + 1, sizeof(char));

  read_to_space(path, HTTP_MAX_PATH_LEN);
  consume_http_version();

  if (path[HTTP_MAX_PATH_LEN - 1] == 0) return path;

  http_fail(404,
            "Not Found",
            "Path is too long, sorry");

  exit(-1);
}

proplist* http_read_headers() {
  proplist* headers = proplist_create();
  char* header_buf = calloc(HTTP_HEADER_CHUNK_LEN + 1, sizeof(char));

  while (1) {
    fgets(header_buf, HTTP_HEADER_CHUNK_LEN, stdin);
    if (strlen(header_buf) <= 2) break;

    char* colon = strstr(header_buf, ": ");

    size_t key_len = colon - header_buf;
    char* key = calloc(key_len + 1, sizeof(char));
    memcpy(key, header_buf, key_len);

    size_t val_len = strlen(colon + 2);
    char* val = calloc(val_len + 1, sizeof(char));
    memcpy(val, colon + 2, val_len);

    proplist_insert(headers, key, val);

    memset(header_buf, 0, HTTP_HEADER_CHUNK_LEN + 1);
  }

  return headers;
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

void http_ok_html_headers() {
  http_puts("HTTP/1.1 200 OK");
  http_send_date_header();
  http_puts("Content-type: text/html");
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

void read_to_space(char* buf, size_t max_len) {
  size_t cursor = 0;

  while (1) {
    char cur = getchar();
    if (cur == ' ') break;
    buf[cursor] = cur;
    cursor++;
    if (cursor > max_len) break;
  }

}

void consume_http_version() {
  char version[80];

  fgets((char*)(&version), 128, stdin);
};