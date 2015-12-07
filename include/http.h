#pragma once

#include "llist.h"
#include "proplist.h"

#define HTTP_MAX_METHOD_LEN 4
#define HTTP_MAX_PATH_LEN 64
#define HTTP_MAX_DATE_LEN 64

#define HTTP_HEADER_CHUNK_LEN 1024

typedef struct {
  char* method;
  char* path;
  proplist* headers;
} http_request;

char* http_read_method();
char* http_read_path();

proplist* http_read_headers();

void http_set_response_header(char* key, char* value);
void http_send_response_headers();

void http_send_date_header();

void http_fail(int code, char* failmesg, char* longmesg);

void http_ok_html_headers();

void http_puts(char* line);
