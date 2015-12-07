#pragma once
#include <sqlite3.h>

#include "http.h"

void service_http_request(sqlite3* db);

void handle_get(sqlite3* db, http_request req);
void handle_post(sqlite3* db, http_request req);
