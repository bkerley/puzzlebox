#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "proplist.h"
#include "http.h"
#include "sqli_handlers.h"
#include "sqlite_util.h"

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
  HH("/", handle_get_index);
  HH("/headers", handle_get_headers);

  char* buf = calloc(1024, sizeof(char));
  snprintf(buf, 1024, "couldn't find path --%s-- wah wah :(\r\n", req.path);

  http_fail(404,
            "Not found",
            buf);

  exit(-1);
}

void handle_get_index(sqlite3* db, http_request req) {
  http_ok_html_headers();

  http_puts("<!doctype html>");
  http_puts("<html><head><title>welcome</title></head><body>");

  printf("<p>the db is at %s and has %d users</p>",
         sqlite3_db_filename(db, "main"),
         count_users(db));

  http_puts("</body></html>");
  http_puts("");
  exit(-1);
}

void handle_get_headers(sqlite3* db, http_request req) {
  http_ok_html_headers();

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