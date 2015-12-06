#pragma once

#include "llist.h"

#define HTTP_MAX_METHOD_LEN 4
#define HTTP_MAX_PATH_LEN 64
#define HTTP_MAX_DATE_LEN 64

#define HTTP_HEADER_CHUNK_LEN 256

typedef struct {
  char* key;
  char* value;
} http_header;

char* http_read_method();
char* http_read_path();

llist* http_read_headers();
void http_consume_headers();

void http_set_response_header(char* key, char* value);
void http_send_response_headers();

void http_send_date_header();

void http_fail(int code, char* failmesg, char* longmesg);

void http_puts(char* line);
