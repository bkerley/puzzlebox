#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include <sqlite3.h>

#include "config.h"
#include "http.h"
#include "sqlite_util.h"
#include "sqli_handlers.h"


int main() {
  chdir(COMPILEDIR);

  with_db(^(sqlite3* db) {
      setup_db(db);

      service_http_request(db);
    });
  return 0;
}
