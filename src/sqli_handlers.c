#include <string.h>
#include <stdlib.h>

#include "http.h"
#include "sqli_handlers.h"

void service_http_request(sqlite3* db) {
  char* method = http_read_method();
  char* path = http_read_path();

  http_consume_headers();

  if (strcmp(method, "GET") == 0) {
    return handle_get(db, path);
  }

  if (strcmp(method, "POST") == 0) {
    return handle_post(db, path);
  }

  http_fail(405,
            "Method Not Allowed",
            "Only GET and POST are supported");

  exit(-1);
}

void handle_get(sqlite3* db, char* path) {
  http_fail(404,
            "Not Found",
            "working");

  exit(-1);
}

void handle_post(sqlite3* db, char* path) {
  http_fail(405,
            "Method Not Allowed",
            "not yet");

  exit(-1);
}
