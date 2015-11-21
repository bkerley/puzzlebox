#include <stdlib.h>
#include <stdio.h>

#include <sqlite3.h>
#include <libscrypt.h>

#include "config.h"

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

int main() {
  with_db(^(sqlite3* db) {
      printf("opened db successfully %p\n", db);

      setup_db(db);

      /* service_http_request(db); */
    });
  return 0;
}

void setup_db(sqlite3* db) {
  just_run(db,
           "CREATE TABLE users ("
           "id INTEGER PRIMARY KEY, "
           "username VARCHAR UNIQUE ON CONFLICT REPLACE, "
           "password_digest VARCHAR);");

  sqlite3_stmt* stmt = NULL;
  sqlite3_stmt** stmt_ptr = &stmt;
  eok(sqlite3_prepare_v2(db,
                         "INSERT INTO users (username, password_digest) "
                         "VALUES (?1, ?2);",
                         -1,
                         stmt_ptr,
                         NULL));


  eok(sqlite3_bind_text(stmt, 1, "admin", -1, SQLITE_STATIC));
  eok(sqlite3_bind_null(stmt, 2));

  eee(SQLITE_DONE, sqlite3_step(stmt));

  eok(sqlite3_reset(stmt));

  eok(sqlite3_bind_text(stmt, 1, VICTIM_USERNAME, -1, SQLITE_STATIC));

  with_password_hash(VICTIM_PASSWORD, ^(char* password_hash) {
      eok(sqlite3_bind_text(stmt, 2, password_hash, -1, SQLITE_TRANSIENT));
    });

  eee(SQLITE_DONE, sqlite3_step(stmt));

  sqlite3_finalize(stmt);
}

void expect_ok(int (^block)()) {
  expect(SQLITE_OK, block);
}

void expect(int expectation, int(^block)()) {
  int lineno = __LINE__;
  char* filename = __FILE__;
  char* errmsg = NULL;

  int result = block(&lineno, &filename, &errmsg);
  if (result == expectation) return;

  printf("Expected 0 (SQLITE_OK), got %d at line %d\n", result, lineno);
  if (errmsg != NULL) {
    puts(errmsg);
  }
  puts("https://sqlite.org/rescode.html");
  exit(-1);
}

void with_db(void (^block)(sqlite3* db)) {
  sqlite3* db = NULL;
  sqlite3 **db_ptr = &db;

  eok(sqlite3_open(DBNAME, db_ptr));

  block(db);
}

void with_password_hash(char* password, void (^block)(char* password_hash)) {
  char* hash_destination = calloc(SCRYPT_MCF_LEN + 1, sizeof(char));

  eee(1, libscrypt_hash(hash_destination,
                        password,
                        SCRYPT_N,
                        SCRYPT_r,
                        SCRYPT_p));

  block(hash_destination);

  free(hash_destination);
}
