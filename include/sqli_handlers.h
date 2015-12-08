#pragma once
#include <sqlite3.h>

#include "http.h"

void service_http_request(sqlite3* db);

void handle_get(sqlite3* db, http_request req);
void handle_post(sqlite3* db, http_request req);

// private-ish

#define HH(pth, func) if (0 == strcmp(req.path, pth)) return func(db, req)

void handle_get_index(sqlite3* db, http_request req);
void handle_get_headers(sqlite3* db, http_request req);
