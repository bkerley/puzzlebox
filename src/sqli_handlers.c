#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "proplist.h"
#include "http.h"
#include "sqli_handlers.h"

void service_http_request(sqlite3* db) {
  http_request req;
  req.method =  http_read_method();
  req.path = http_read_path();
  req.headers = http_read_headers();

  if (strcmp(req.method, "GET") == 0) {
    return handle_get(db, req);
  }

  if (strcmp(req.method, "POST") == 0) {
    return handle_post(db, req);
  }

  http_fail(405,
            "Method Not Allowed",
            "Only GET and POST are supported");

  exit(-1);
}

void handle_get(sqlite3* db, http_request req) {
  http_puts("<!doctype html>");
  http_puts("<html><head><title>ayyyy</title></head><body>");
  http_puts("<dl>");

  proplist_each(req.headers, ^(char* key, char* value){

      printf("<dt>%s</dt><dd>%s</dd>\r\n", key, value);

    });

  http_puts("</dl></body></html>");
  http_puts("");

  exit(-1);
}

void handle_post(sqlite3* db, http_request req) {
  http_fail(405,
            "Method Not Allowed",
            "not yet");

  exit(-1);
}
