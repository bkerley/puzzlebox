#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <syslog.h>

#include <sqlite3.h>
#include <libscrypt.h>

#include "config.h"
#include "http.h"
#include "sqlite_util.h"

void setup_db(sqlite3* db) {
  just_run(db,
           "CREATE TABLE IF NOT EXISTS users ("
           "id INTEGER PRIMARY KEY, "
           "username VARCHAR UNIQUE ON CONFLICT REPLACE, "
           "password_digest VARCHAR);");
  syslog(LOG_MAKEPRI(LOG_USER, LOG_INFO), "created users table");

  just_run(db, "INSERT INTO users (username, password_digest) VALUES ('admin', 'admin')");

  just_run(db, "BEGIN TRANSACTION;");

  sqlite3_stmt* stmt = NULL;
  sqlite3_stmt** stmt_ptr = &stmt;
  eok(sqlite3_prepare_v2(db,
                         "INSERT INTO users (username, password_digest) "
                         "VALUES (?1, ?2);",
                         -1,
                         stmt_ptr,
                         NULL));
  syslog(LOG_MAKEPRI(LOG_USER, LOG_INFO), "prepared user insert");

  eok(sqlite3_bind_text(stmt, 1, "admin", -1, SQLITE_STATIC));
  eok(sqlite3_bind_null(stmt, 2));

  eee(SQLITE_DONE, sqlite3_step(stmt));
  syslog(LOG_MAKEPRI(LOG_USER, LOG_INFO), "stepped admin");

  eok(sqlite3_reset(stmt));

  eok(sqlite3_bind_text(stmt, 1, VICTIM_USERNAME, -1, SQLITE_STATIC));

  with_password_hash(VICTIM_PASSWORD, ^(char* password_hash) {
      eok(sqlite3_bind_text(stmt, 2, password_hash, -1, SQLITE_TRANSIENT));
    });

  eee(SQLITE_DONE, sqlite3_step(stmt));
  syslog(LOG_MAKEPRI(LOG_USER, LOG_INFO), "stepped user");

  sqlite3_finalize(stmt);
  syslog(LOG_MAKEPRI(LOG_USER, LOG_INFO), "finalized");

  just_run(db, "COMMIT TRANSACTION;");
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

  char* errbuf = calloc(sizeof(char), ERR_BUF_SIZE);

  snprintf(errbuf,
           ERR_BUF_SIZE - 1,
           "Expected %d, got %d at line %d.\n",
           expectation, result, lineno);

  if (errmsg != NULL) {
    strncat(errbuf, errmsg, ERR_BUF_SIZE - 1);
  }

  http_fail(500,
            "Internal Server Error",
            errbuf);

  exit(-1);
}

void with_db(void (^block)(sqlite3* db)) {
  sqlite3* db = NULL;
  sqlite3 **db_ptr = &db;

  eok(sqlite3_open(DBNAME, db_ptr));

  block(db);

  eok(sqlite3_close(db));
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

int count_users(sqlite3* db) {
  sqlite3_stmt* stmt = NULL;
  sqlite3_stmt** stmt_ptr = &stmt;
  eok(sqlite3_prepare_v2(db,
                         "SELECT COUNT(*) FROM users;",
                         -1,
                         stmt_ptr,
                         NULL));

  eee(SQLITE_ROW, sqlite3_step(stmt));

  eee(1, sqlite3_column_count(stmt));
  eee(SQLITE_INTEGER, sqlite3_column_type(stmt, 0));

  return sqlite3_column_int(stmt, 0);
}
