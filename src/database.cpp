#include "database.hpp"

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>

#include "sqlite3.h"

namespace invoice_maker::DB {
static sqlite3 *CONNECTION = NULL;

int connect() {
    int flags =
        SQLITE_OPEN_FULLMUTEX | SQLITE_OPEN_CREATE | SQLITE_OPEN_READWRITE;

    int result_code = sqlite3_open_v2(u8"file:database?journal mode=WAL",
                                      &CONNECTION, flags, NULL);

    if (result_code) {
        fprintf(stderr, "DB Error: %s\n", sqlite3_errmsg(CONNECTION));

        return 1;
    }

    return 0;
}

int destroy() {
    if (!CONNECTION) {
        return 0;
    }

    return sqlite3_close(CONNECTION);
}

}  // namespace invoice_maker::DB

void db_do_something() {
    sqlite3 *db = NULL;

    int code = sqlite3_open(u8"database", &db);
    if (code) {
        std::cout << "Chyba: " << sqlite3_errmsg(db) << "CO KURVA" << std::endl;
        return;
    }

    if (db == NULL) {
        fprintf(stderr, "Failed to open sqlite connection!\n");
        exit(1);
    }

    const char *query =
        u8"CREATE TABLE IF NOT EXISTS Users ( id BIGINT PRIMARY KEY );";

    char *err_msg = NULL;

    auto callback = [](void *, int n_cols, char **values, char **column_names) {
        std::cout << "Něco se stalo!" << std::endl;

        for (int i = 0; i < n_cols; i++) {
            printf("%s = %s\n", column_names[i],
                   values[i] ? values[i] : "NULL");
        }

        return 0;
    };

    code = sqlite3_exec(db, query, callback, NULL, &err_msg);
    if (code != SQLITE_OK) {
        std::cout << "Chyba: " << err_msg << std::endl;
        sqlite3_free(err_msg);
    }

    sqlite3_close(db);
}
