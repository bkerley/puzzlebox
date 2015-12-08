#pragma once

#include <sqlite3.h>
#include <stdlib.h>

#define ERR_BUF_SIZE 0x1000

#define eok_args ^(int* lineno_ptr, char** filename_ptr, char** errmsg)
#define eok_snap (*lineno_ptr = __LINE__, *filename_ptr = __FILE__)

#define eok(x) expect_ok(eok_args{eok_snap; return x;})
#define eee(x, y) expect(x, eok_args{eok_snap; return x;})

#define just_run(db, x) eok(sqlite3_exec(db, x, NULL, NULL, errmsg));

void expect_ok(int (^block)());
void expect(int expectation, int (^block)());
void with_db(void (^block)(sqlite3* db));
void setup_db(sqlite3* db);
void with_password_hash(char* password, void (^block)(char* password_hash));

int count_users(sqlite3* db);
