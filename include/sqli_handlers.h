#pragma once
#include <sqlite3.h>

void service_http_request(sqlite3* db);

void handle_get(sqlite3* db, char* path);
void handle_post(sqlite3* db, char* path);
