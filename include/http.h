#pragma once

#define HTTP_MAX_METHOD_LEN 4
#define HTTP_MAX_PATH_LEN 64
#define HTTP_MAX_DATE_LEN 64

#define HTTP_HEADER_CHUNK_LEN 256


char* http_read_method();
char* http_read_path();

void http_consume_headers();

void http_send_date_header();

void http_puts(char* line);
